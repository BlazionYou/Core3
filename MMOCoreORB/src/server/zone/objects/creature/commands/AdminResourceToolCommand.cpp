/*
		Copyright <BlazinYou>
	See file COPYING for copying conditions.*/

#include "AdminResourceToolCommand.h"
#include "server/zone/objects/player/sui/callbacks/AdminResourceToolSuiCallback.h"
#include "server/zone/managers/resource/ResourceManager.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/ZoneProcessServer.h"

int AdminResourceToolCommand::doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

	if (!checkStateMask(creature))
		return INVALIDSTATE;

	if (!checkInvalidLocomotions(creature))
		return INVALIDLOCOMOTION;

	// Check if player is admin
	ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();
	if (ghost == nullptr || !ghost->isPrivileged()) {
		creature->sendSystemMessage("You must be an admin to use this command.");
		return GENERALERROR;
	}

	ZoneServer* zoneServer = creature->getZoneServer();
	if (zoneServer == nullptr) {
		return GENERALERROR;
	}

	// Parse arguments to determine initial action
	String args = arguments.toString();
	
	// If no arguments, show main menu
	if (args.isEmpty() || args == "menu") {
		showMainMenu(creature, zoneServer);
	} else if (args == "planet") {
		// Show planet selection
		showPlanetSelection(creature, zoneServer);
	} else if (args.beginsWith("planet:")) {
		// Show resources for specific planet
		String planet = args.replaceFirst("planet:", "");
		showResourcesForPlanet(creature, zoneServer, planet);
	} else if (args == "history") {
		// Show resource history
		showResourceHistory(creature, zoneServer);
	} else if (args == "spawn") {
		// Show spawn menu
		showSpawnMenu(creature, zoneServer);
	} else if (args == "give") {
		// Show give resource menu
		showGiveResourceMenu(creature, zoneServer);
	} else if (args.beginsWith("despawn:")) {
		// Despawn a resource
		String resourceName = args.replaceFirst("despawn:", "");
		despawnResource(creature, zoneServer, resourceName);
	} else if (args == "help") {
		showHelp(creature);
	} else {
		// Default to main menu
		showMainMenu(creature, zoneServer);
	}

	return SUCCESS;
}

void AdminResourceToolCommand::showMainMenu(CreatureObject* creature, ZoneServer* zoneServer) const {
	// Create the main SUI window
	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Admin Resource Tool");
	sui->setPromptText("Select an option from the menu below.\n\n"
		"Use /adminresources planet <planetname> to view resources on a specific planet.\n"
		"Use /adminresources history to view resource history.\n"
		"Use /adminresources spawn to spawn new resources.\n"
		"Use /adminresources give to give resources to players.\n"
		"Use /adminresources despawn:<resourcename> to despawn a resource.");

	// Add menu items for different sections
	sui->addMenuItem("=== VIEW RESOURCES ===");
	sui->addMenuItem("Planet: Tatooine");
	sui->addMenuItem("Planet: Corellia");
	sui->addMenuItem("Planet: Naboo");
	sui->addMenuItem("Planet: Dantooine");
	sui->addMenuItem("Planet: Dathomir");
	sui->addMenuItem("Planet: Endor");
	sui->addMenuItem("Planet: Rori");
	sui->addMenuItem("Planet: Talus");
	sui->addMenuItem("Planet: Lok");
	sui->addMenuItem("Planet: Yavin4");
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

	// Add callback
	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));

	// Add buttons
	sui->setOkButton(true, "Select");
	sui->setCancelButton(true, "Close");

	// Send the SUI to the player
	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolCommand::showPlanetSelection(CreatureObject* creature, ZoneServer* zoneServer) const {
	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Select Planet");
	sui->setPromptText("Select a planet to view its resources:");

	sui->addMenuItem("Tatooine");
	sui->addMenuItem("Corellia");
	sui->addMenuItem("Naboo");
	sui->addMenuItem("Dantooine");
	sui->addMenuItem("Dathomir");
	sui->addMenuItem("Endor");
	sui->addMenuItem("Rori");
	sui->addMenuItem("Talus");
	sui->addMenuItem("Lok");
	sui->addMenuItem("Yavin4");

	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
	sui->setOkButton(true, "View");
	sui->setCancelButton(true, "Back");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolCommand::showResourcesForPlanet(CreatureObject* creature, ZoneServer* zoneServer, const String& planet) const {
	ManagedReference<ResourceManager*> resourceManager = zoneServer->getResourceManager();
	if (resourceManager == nullptr) {
		creature->sendSystemMessage("Error: Resource manager not available.");
		return;
	}

	// Get resources for this planet
	Vector<ManagedReference<ResourceSpawn*> > resourceList;
	
	// Use -1 to get all types, and the planet name
	resourceManager->getResourceListByType(resourceList, -1, planet);

	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Resources on " + planet);
	
	StringBuffer prompt;
	prompt << "Active resources on " << planet << ":\n\n";
	prompt << "Select a resource for more options.\n";
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
	sui->setOkButton(true, "Details");
	sui->setCancelButton(true, "Back");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolCommand::showResourceHistory(CreatureObject* creature, ZoneServer* zoneServer) const {
	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Resource History");
	sui->setPromptText("Resource history tracking is maintained in memory.\n\n"
		"This feature shows all resources that have been spawned since server start.\n"
		"Use the main menu 'View Resource History' option to see tracked resources.");

	sui->addMenuItem("Note: Historical tracking requires server-side implementation.");
	sui->addMenuItem("Current resources can be viewed per planet.");

	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
	sui->setOkButton(true, "OK");
	sui->setCancelButton(true, "Back");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolCommand::showSpawnMenu(CreatureObject* creature, ZoneServer* zoneServer) const {
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
	sui->addMenuItem("Spawn Enhanced Resource (with boosted stats)");

	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
	sui->setOkButton(true, "OK");
	sui->setCancelButton(true, "Back");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolCommand::showGiveResourceMenu(CreatureObject* creature, ZoneServer* zoneServer) const {
	ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
	sui->setPromptTitle("Give Resource to Player");
	sui->setPromptText("To give resources to a player:\n\n"
		"1. Use survey tool to harvest resources into a container\n"
		"2. Give the container to the player\n\n"
		"Or use these methods:\n"
		"- Use Resource Container items from Character Builder\n"
		"- Spawn resources and let players harvest them\n\n"
		"Direct spawn: /gmCreateSpecificResource <name> <planet>");

	sui->addMenuItem("Use Character Builder for resource containers");
	sui->addMenuItem("Spawn resources for players to harvest");
	sui->addMenuItem("Use /gmCreateSpecificResource command");

	sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
	sui->setOkButton(true, "OK");
	sui->setCancelButton(true, "Back");

	creature->sendMessage(sui->generateMessage());
}

void AdminResourceToolCommand::despawnResource(CreatureObject* creature, ZoneServer* zoneServer, const String& resourceName) const {
	ManagedReference<ResourceManager*> resourceManager = zoneServer->getResourceManager();
	if (resourceManager == nullptr) {
		creature->sendSystemMessage("Error: Resource manager not available.");
		return;
	}

	String result = resourceManager->despawnResource(const_cast<String&>(resourceName));
	
	if (!result.isEmpty() && result != "false") {
		creature->sendSystemMessage("Successfully despawned: " + resourceName);
	} else {
		creature->sendSystemMessage("Failed to despawn: " + resourceName);
	}
}

void AdminResourceToolCommand::showHelp(CreatureObject* creature) const {
	creature->sendSystemMessage("Admin Resource Tool Commands:");
	creature->sendSystemMessage("/adminresources - Open main menu");
	creature->sendSystemMessage("/adminresources planet - Select planet to view");
	creature->sendSystemMessage("/adminresources planet:<name> - View resources on planet");
	creature->sendSystemMessage("/adminresources history - View resource history");
	creature->sendSystemMessage("/adminresources spawn - Spawn new resource");
	creature->sendSystemMessage("/adminresources give - Give resource to player");
	creature->sendSystemMessage("/adminresources despawn:<name> - Despawn a resource");
	creature->sendSystemMessage("/adminresources help - Show this help");
}
