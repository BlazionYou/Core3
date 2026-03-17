/*
		Copyright <BlazinYou>
	See file COPYING for copying conditions.*/

#include "AdminResourceToolSuiCallback.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/objects/player/sui/inputbox/SuiInputBox.h"
#include "server/zone/managers/resource/ResourceManager.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/resource/ResourceSpawn.h"

void AdminResourceToolSuiCallback::run(CreatureObject* creature, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args) {
	bool cancelPressed = (eventIndex == 1);
	
	if (!sui->isListBox() || cancelPressed || server == nullptr)
		return;

	if (!creature->isPlayerCreature())
		return;

	ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();
	if (ghost == nullptr || !ghost->isPrivileged()) {
		creature->sendSystemMessage("You must be an admin to use this command.");
		return;
	}

	SuiListBox* listBox = cast<SuiListBox*>(sui);
	int index = Integer::valueOf(args->get(0).toString());

	if (index < 0)
		return;

	String selection = listBox->getMenuItemName(index);
	ZoneServer* zoneServer = server;

	// Handle main menu selections - extract lowercase planet name from "Planet: Name (lowercase)"
	if (selection.contains("Planet:")) {
		// Extract the lowercase name from "Planet: Tatooine (tatooine)"
		int parenStart = selection.lastIndexOf("(");
		int parenEnd = selection.lastIndexOf(")");
		if (parenStart > 0 && parenEnd > parenStart) {
			String planet = selection.subString(parenStart + 1, parenEnd);
			showResourcesForPlanet(creature, zoneServer, planet);
		} else {
			// Fallback: just use what comes after "Planet: "
			String planet = selection.replaceFirst("Planet: ", "");
			showResourcesForPlanet(creature, zoneServer, planet);
		}
	} else if (selection == "View Resource History") {
		showResourceHistory(creature, zoneServer);
	} else if (selection == "Spawn New Resource") {
		showSpawnHelp(creature, zoneServer);
	} else if (selection == "Spawn Enhanced Resource") {
		showSpawnEnhancedMenu(creature, zoneServer);
	} else if (selection == "Give Resource to Player") {
		showGiveResourceMenu(creature, zoneServer);
	} else if (selection == "Despawn Resource") {
		promptDespawnResource(creature, zoneServer);
	} else if (selection == "Refresh All Data") {
		showMainMenu(creature, zoneServer);
	} else if (selection.contains(" - ")) {
		// This is likely a resource entry - show details
		String resourceName = selection.subString(0, selection.indexOf(" - "));
		showResourceDetails(creature, zoneServer, resourceName);
	} else if (selection == "View Details" || selection == "Details") {
		// Get first selected item - use the index from args
		if (index >= 0 && index < listBox->getMenuSize()) {
			String item = listBox->getMenuItemName(index);
			if (item.contains(" - ")) {
				String resourceName = item.subString(0, item.indexOf(" - "));
				showResourceDetails(creature, zoneServer, resourceName);
			}
		}
	} else if (selection.contains("(") && selection.contains(")")) {
		// Planet selection from the planet list - extract lowercase name from "Tatooine (tatooine)"
		int parenStart = selection.lastIndexOf("(");
		int parenEnd = selection.lastIndexOf(")");
		if (parenStart > 0 && parenEnd > parenStart) {
			String planet = selection.subString(parenStart + 1, parenEnd);
			showResourcesForPlanet(creature, zoneServer, planet);
		}
	} else if (selection.contains("Use /gmCreateSpecificResource")) {
		showSpawnHelp(creature, zoneServer);
	} else if (selection.contains("Use /gmCreateClassResource")) {
		showSpawnHelp(creature, zoneServer);
	} else if (selection.contains("Use Character Builder")) {
		creature->sendSystemMessage("Open Character Builder (Ctrl+Shift+B) and search for 'Resource Container'");
	} else if (selection.contains("Spawn resources for players")) {
		showSpawnHelp(creature, zoneServer);
	} else if (selection == "Back to Main Menu" || selection == "Back") {
		showMainMenu(creature, zoneServer);
	} else if (selection == "OK" || selection == "Close") {
		// Just close the window
		return;
	} else {
		// Default action - show main menu
		showMainMenu(creature, zoneServer);
	}
}

void AdminResourceToolSuiCallback::showMainMenu(CreatureObject* creature, ZoneServer* zoneServer) const {
	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Admin Resource Tool");
	sui->setPromptText("Select an option from the menu below.\n\n"
		"Use /adminresources planet <planetname> to view resources on a specific planet.\n"
		"Use /adminresources history to view resource history.\n"
		"Use /adminresources spawn to spawn new resources.\n"
		"Use /adminresources give to give resources to players.\n"
		"Use /adminresources despawn:<resourcename> to despawn a resource.");

	// Add menu items for different sections (display names capitalized, but internal names lowercase)
	sui->addMenuItem("=== VIEW RESOURCES ===");
	sui->addMenuItem("Planet: Tatooine (tatooine)");
	sui->addMenuItem("Planet: Corellia (corellia)");
	sui->addMenuItem("Planet: Naboo (naboo)");
	sui->addMenuItem("Planet: Dantooine (dantooine)");
	sui->addMenuItem("Planet: Dathomir (dathomir)");
	sui->addMenuItem("Planet: Endor (endor)");
	sui->addMenuItem("Planet: Rori (rori)");
	sui->addMenuItem("Planet: Talus (talus)");
	sui->addMenuItem("Planet: Lok (lok)");
	sui->addMenuItem("Planet: Yavin4 (yavin4)");
	sui->addMenuItem("");
	sui->addMenuItem("=== TOOLS ===");
	sui->addMenuItem("View Resource History");
	sui->addMenuItem("Spawn New Resource");
	sui->addMenuItem("Spawn Enhanced Resource");
	sui->addMenuItem("Give Resource to Player");
	sui->addMenuItem("Despawn Resource");
	sui->addMenuItem("");
	sui->addMenuItem("=== REFRESH ===");
	sui->addMenuItem("Refresh All Data");

	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
	sui->setOkButton(true, "Select");
	sui->setCancelButton(true, "Close");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolSuiCallback::showResourcesForPlanet(CreatureObject* creature, ZoneServer* zoneServer, const String& planet) const {
	ManagedReference<ResourceManager*> resourceManager = zoneServer->getResourceManager();
	if (resourceManager == nullptr) {
		creature->sendSystemMessage("Error: Resource manager not available.");
		return;
	}

	// Get resources for this planet
	Vector<ManagedReference<ResourceSpawn*> > resourceList;
	resourceManager->getResourceListByType(resourceList, -1, planet);

	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Resources on " + planet);
	
	StringBuffer prompt;
	prompt << "Active resources on " << planet << ":\n\n";
	prompt << "Select a resource for more details.\n";
	prompt << "Format: [Name] - [Class] - [Time Remaining]";
	sui->setPromptText(prompt.toString());

	time_t currentTime = time(nullptr);

	for (int i = 0; i < resourceList.size(); ++i) {
		ManagedReference<ResourceSpawn*> resource = resourceList.get(i);
		if (resource == nullptr)
			continue;

		String resourceName = resource->getName();
		String resourceClass = resource->getFinalClass();
		
		unsigned long despawnTime = resource->getDespawned();
		String timeRemaining;
		
		if (despawnTime > currentTime) {
			int secondsLeft = despawnTime - currentTime;
			int hours = secondsLeft / 3600;
			int minutes = (secondsLeft % 3600) / 60;
			
			if (hours > 0) {
				timeRemaining = String::valueOf(hours) + "h " + String::valueOf(minutes) + "m";
			} else {
				timeRemaining = String::valueOf(minutes) + "m";
			}
		} else {
			timeRemaining = "EXPIRED";
		}

		String display = resourceName + " - " + resourceClass + " - " + timeRemaining;
		sui->addMenuItem(display);
	}

	if (resourceList.size() == 0) {
		sui->addMenuItem("No active resources on this planet.");
	}

	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
	sui->setOkButton(true, "View Details");
	sui->setCancelButton(true, "Back to Main Menu");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolSuiCallback::showResourceDetails(CreatureObject* creature, ZoneServer* zoneServer, const String& resourceName) const {
	ManagedReference<ResourceManager*> resourceManager = zoneServer->getResourceManager();
	if (resourceManager == nullptr) {
		creature->sendSystemMessage("Error: Resource manager not available.");
		return;
	}

	ManagedReference<ResourceSpawn*> resource = resourceManager->getResourceSpawn(resourceName);
	if (resource == nullptr) {
		creature->sendSystemMessage("Resource not found: " + resourceName);
		return;
	}

	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Resource: " + resourceName);
	
	StringBuffer prompt;
	prompt << "Name: " << resource->getName() << "\n";
	prompt << "Type: " << resource->getType() << "\n";
	prompt << "Class: " << resource->getFinalClass() << "\n\n";
	prompt << "=== Stats ===";
	sui->setPromptText(prompt.toString());

	// Add stats
	String attributes[] = {"res_cold_resist", "res_conductivity", "res_decay_resist", "res_heat_resist", 
						   "res_flavor", "res_malleability", "res_potential_energy", "res_quality", 
						   "res_shock_resistance", "res_toughness"};
	String statNames[] = {"Cold Resistance (CR)", "Conductivity (CD)", "Decay Resistance (DR)", "Heat Resistance (HR)",
						 "Flavor (FL)", "Malleability (MA)", "Potential Energy (PE)", "Quality (OQ)", 
						 "Shock Resistance (SR)", "Toughness (UT)"};

	for (int i = 0; i < 10; ++i) {
		int value = resource->getValueOf(attributes[i]);
		String display = statNames[i] + ": " + String::valueOf(value);
		sui->addMenuItem(display);
	}

	// Add actions
	sui->addMenuItem("");
	sui->addMenuItem("=== ACTIONS ===");
	sui->addMenuItem("Despawn This Resource");
	sui->addMenuItem("View on Another Planet");

	time_t currentTime = time(nullptr);
	unsigned long despawnTime = resource->getDespawned();
	if (despawnTime > currentTime) {
		int secondsLeft = despawnTime - currentTime;
		int hours = secondsLeft / 3600;
		int minutes = (secondsLeft % 3600) / 60;
		creature->sendSystemMessage("Time remaining for " + resourceName + ": " + String::valueOf(hours) + "h " + String::valueOf(minutes) + "m");
	}

	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
	sui->setOkButton(true, "OK");
	sui->setCancelButton(true, "Back");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolSuiCallback::showResourceHistory(CreatureObject* creature, ZoneServer* zoneServer) const {
	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Resource History");
	sui->setPromptText("Resource history tracking is maintained in memory.\n\n"
		"This feature shows all resources that have been spawned since server start.\n"
		"Note: Full historical tracking requires server-side state management.");

	sui->addMenuItem("Current resources shown per planet in the main menu.");
	sui->addMenuItem("Use /adminresources planet:<name> to view specific planets.");
	sui->addMenuItem("");
	sui->addMenuItem("Note: History tracking is planned for future implementation.");

	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
	sui->setOkButton(true, "OK");
	sui->setCancelButton(true, "Back to Main Menu");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolSuiCallback::showSpawnHelp(CreatureObject* creature, ZoneServer* zoneServer) const {
	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Spawn Resource");
	sui->setPromptText("Resource Spawning Options:\n\n"
		"Use the following GM commands to spawn resources:\n\n"
		"/gmCreateSpecificResource <resourcename> <planet> - Spawn a specific resource\n"
		"/gmCreateClassResource <classname> <planet> - Spawn a resource of a class\n\n"
		"Example: /gmCreateSpecificResource agricultural_wool_rice tatooine\n"
		"Example: /gmCreateClassResource mineral_crystalline tatooine");

	sui->addMenuItem("Use /gmCreateSpecificResource command");
	sui->addMenuItem("Use /gmCreateClassResource command");
	sui->addMenuItem("Back to Main Menu");

	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
	sui->setOkButton(true, "OK");
	sui->setCancelButton(true, "Back");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolSuiCallback::showSpawnEnhancedMenu(CreatureObject* creature, ZoneServer* zoneServer) const {
	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Spawn Enhanced Resource");
	sui->setPromptText("Spawn Enhanced Resource with boosted stats:\n\n"
		"This feature allows you to create resources with stats higher than normal.\n"
		"The system finds the historical best value and applies a boost percentage.\n\n"
		"Boost calculation: NewValue = HistoricalBest * (1 + BoostPercent/100)\n"
		"Example: Best OQ was 980, boost 0.1% = 981\n\n"
		"Use: /gmCreateSpecificResource <resourcename> <planet>\n"
		"Then use the resource tool to view and manage it.");

	sui->addMenuItem("Use /gmCreateSpecificResource to spawn base resource");
	sui->addMenuItem("Enhanced spawning planned for future implementation");
	sui->addMenuItem("Back to Main Menu");

	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
	sui->setOkButton(true, "OK");
	sui->setCancelButton(true, "Back");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolSuiCallback::showGiveResourceMenu(CreatureObject* creature, ZoneServer* zoneServer) const {
	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Give Resource to Player");
	sui->setPromptText("To give resources to a player:\n\n"
		"1. Use survey tool to harvest resources into a container\n"
		"2. Give the container to the player\n\n"
		"Or use these methods:\n"
		"- Use Resource Container items from Character Builder (Ctrl+Shift+B)\n"
		"- Spawn resources and let players harvest them\n\n"
		"Direct spawn: /gmCreateSpecificResource <name> <planet>");

	sui->addMenuItem("Use Character Builder for resource containers");
	sui->addMenuItem("Spawn resources for players to harvest");
	sui->addMenuItem("Use /gmCreateSpecificResource command");
	sui->addMenuItem("Back to Main Menu");

	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
	sui->setOkButton(true, "OK");
	sui->setCancelButton(true, "Back");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolSuiCallback::promptDespawnResource(CreatureObject* creature, ZoneServer* zoneServer) const {
	ManagedReference<SuiInputBox*> inputSui = new SuiInputBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL);
	inputSui->setPromptTitle("Despawn Resource");
	inputSui->setPromptText("Enter the exact name of the resource to despawn:");
	inputSui->setCancelButton(true, "Cancel");
	inputSui->setOkButton(true, "Despawn");

	inputSui->setCallback(new AdminResourceToolSuiCallback(zoneServer));

	creature->sendMessage(inputSui->generateMessage());
}
