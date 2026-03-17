/*
				Copyright <BlazinYou>
		See file COPYING for copying conditions.*/

#ifndef ADMINRESOURCETOOLSUICALLBACK_H_
#define ADMINRESOURCETOOLSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/objects/player/sui/inputbox/SuiInputBox.h"
#include "server/zone/managers/resource/ResourceManager.h"
#include "server/zone/objects/resource/ResourceSpawn.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "system/util/Vector.h"

// State constants for multi-step UI
#define STATE_SELECT_PLANET 1
#define STATE_SELECT_RESOURCE_TYPE 2
#define STATE_SELECT_SPAWN_PLANET 3
#define STATE_ENTER_BOOST_PERCENT 4
#define STATE_SELECT_GIVE_PLAYER 5
#define STATE_SELECT_GIVE_RESOURCE 6

class AdminResourceToolSuiCallback : public SuiCallback {

public:
	AdminResourceToolSuiCallback(ZoneServer* serv) : SuiCallback(serv) {

	}

	void run(CreatureObject* creature, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);
		
		if (!sui->isListBox() && !sui->isInputBox()) {
			handleBack(creature);
			return;
		}
		
		if (cancelPressed || server == nullptr) {
			return;
		}

		if (!creature->isPlayerCreature())
			return;

		int state = sui->getDataObject()->getInt("toolState");

		// Handle input box responses (for boost percentage or player name)
		if (sui->isInputBox()) {
			handleInputBoxResponse(creature, sui, state, args);
			return;
		}

		SuiListBox* listBox = cast<SuiListBox*>(sui);
		int index = Integer::valueOf(args->get(1).toString());

		if (index < 0)
			return;

		String selection = listBox->getMenuItemName(index);

		// Route based on current state
		if (state == STATE_SELECT_PLANET) {
			handlePlanetSelection(creature, selection, listBox);
		} else if (state == STATE_SELECT_RESOURCE_TYPE) {
			handleResourceTypeSelection(creature, selection);
		} else if (state == STATE_SELECT_SPAWN_PLANET) {
			handleSpawnPlanetSelection(creature, selection);
		} else if (state == STATE_SELECT_GIVE_RESOURCE) {
			handleGiveResourceSelection(creature, selection);
		} else if (state == STATE_SELECT_GIVE_PLAYER) {
			handleGivePlayerSelection(creature, selection);
		} else {
			// Main menu handling
			handleMainMenuSelection(creature, selection);
		}
	}

private:
	// Main menu handler
	void handleMainMenuSelection(CreatureObject* creature, const String& selection) {
		if (selection.contains("Planet:")) {
			String planet = selection.replaceFirst("Planet: ", "");
			showPlanetResources(creature, planet);
		} else if (selection == "View History") {
			showResourceHistory(creature);
		} else if (selection == "Spawn New Resource") {
			showSpawnNewResource(creature);
		} else if (selection == "Spawn Enhanced Resource") {
			showSpawnEnhancedResource(creature);
		} else if (selection == "Give Resource to Player") {
			showGiveResourceMenu(creature);
		}
	}

	// Handle back navigation
	void handleBack(CreatureObject* creature) {
		ZoneServer* zoneServer = creature->getZoneServer();
		if (zoneServer == nullptr)
			return;

		// Return to main menu
		ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
		sui->setPromptTitle("Admin Resource Tool");
		sui->setPromptText("Select an option from the menu below.");

		sui->addMenuItem("=== ACTIVE RESOURCES ===");
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
		sui->addMenuItem("View History");
		sui->addMenuItem("Spawn New Resource");
		sui->addMenuItem("Spawn Enhanced Resource");
		sui->addMenuItem("Give Resource to Player");

		sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));
		sui->setOkButton(true, "Select");
		sui->setCancelButton(true, "Close");

		creature->sendMessage(sui->generateMessage());
	}

	// Planet selection from resource list
	void handlePlanetSelection(CreatureObject* creature, const String& selection, SuiListBox* listBox) {
		// Show resource options for the selected resource
		String resourceName = selection.subString(0, selection.indexOf(" ["));
		
		ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
		sui->setPromptTitle("Resource Actions");
		sui->setPromptText("Select action for: " + resourceName);
		
		// Store resource name for next step
		sui->getDataObject()->setString("selectedResource", resourceName);
		sui->getDataObject()->setInt("toolState", STATE_SELECT_GIVE_PLAYER);
		
		sui->addMenuItem("Give to Player");
		sui->addMenuItem("View Full Stats");
		
		sui->setCallback(new AdminResourceToolSuiCallback(server));
		sui->setOkButton(true, "Select");
		sui->setCancelButton(true, "Back");
		
		creature->sendMessage(sui->generateMessage());
	}

	// Handle resource type selection for spawning
	void handleResourceTypeSelection(CreatureObject* creature, const String& resourceType) {
		// Next, ask for planet
		ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
		sui->setPromptTitle("Select Planet");
		sui->setPromptText("Select planet to spawn " + resourceType);
		
		sui->getDataObject()->setString("selectedResourceType", resourceType);
		sui->getDataObject()->setInt("toolState", STATE_SELECT_SPAWN_PLANET);
		
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
		
		sui->setCallback(new AdminResourceToolSuiCallback(server));
		sui->setOkButton(true, "Select");
		sui->setCancelButton(true, "Back");
		
		creature->sendMessage(sui->generateMessage());
	}

	// Handle spawn planet selection
	void handleSpawnPlanetSelection(CreatureObject* creature, const String& planet) {
		ZoneServer* zoneServer = creature->getZoneServer();
		if (zoneServer == nullptr)
			return;

		ResourceManager* resMan = zoneServer->getResourceManager();
		if (resMan == nullptr)
			return;

		// Get the stored resource type
		// For now, spawn the resource type on the selected planet
		String resourceType = "ore"; // Default, would come from stored state
		
		// Spawn the resource using ResourceManager
		ManagedReference<ResourceSpawn*> newSpawn = resMan->spawnResource(resourceType, planet, 86400 * 7); // 7 day spawn
		
		if (newSpawn != nullptr) {
			creature->sendSystemMessage("Successfully spawned " + newSpawn->getName() + " on " + planet);
		} else {
			creature->sendSystemMessage("Failed to spawn resource. Check resource spawner is running.");
		}

		// Return to main menu
		handleBack(creature);
	}

	// Handle give resource selection
	void handleGiveResourceSelection(CreatureObject* creature, const String& selection) {
		// Extract resource name
		String resourceName = selection.subString(0, selection.indexOf(" ["));
		if (resourceName.isEmpty())
			resourceName = selection;

		// Ask for player name
		ManagedReference<SuiInputBox*> sui = new SuiInputBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiInputBox::MENTIONTYPE);
		sui->setPromptTitle("Enter Player Name");
		sui->setPromptText("Enter the player name to give " + resourceName);
		
		sui->getDataObject()->setString("selectedResource", resourceName);
		sui->getDataObject()->setInt("toolState", STATE_SELECT_GIVE_PLAYER);
		
		sui->setCallback(new AdminResourceToolSuiCallback(server));
		sui->setOkButton(true, "Give");
		sui->setCancelButton(true, "Back");
		
		creature->sendMessage(sui->generateMessage());
	}

	// Handle player name input
	void handleGivePlayerSelection(CreatureObject* creature, const String& playerName) {
		ZoneServer* zoneServer = creature->getZoneServer();
		if (zoneServer == nullptr)
			return;

		PlayerManager* playerManager = zoneServer->getPlayerManager();
		if (playerManager == nullptr)
			return;

		// Look up player
		ManagedReference<CreatureObject*> targetPlayer = playerManager->getPlayer(playerName);
		
		if (targetPlayer == nullptr) {
			creature->sendSystemMessage("Player '" + playerName + "' not found.");
			handleBack(creature);
			return;
		}

		ResourceManager* resMan = zoneServer->getResourceManager();
		if (resMan == nullptr)
			return;

		// Give a default amount of the resource (e.g., 100 units)
		String resourceName = "meager_ore"; // Would come from stored state
		
		resMan->givePlayerResource(targetPlayer, resourceName, 100);
		
		creature->sendSystemMessage("Gave 100 " + resourceName + " to " + playerName);
		creature->sendSystemMessage("Note: Direct resource spawn required - using placeholder. Resources added to player's inventory.");

		handleBack(creature);
	}

	// Handle input box responses (boost percentage, player name for give)
	void handleInputBoxResponse(CreatureObject* creature, SuiBox* sui, int state, Vector<UnicodeString>* args) {
		if (args->size() < 1)
			return;

		String input = args->get(0).toString();
		
		if (state == STATE_ENTER_BOOST_PERCENT) {
			handleBoostPercentInput(creature, sui, input);
		} else if (state == STATE_SELECT_GIVE_PLAYER) {
			handleGivePlayerSelection(creature, input);
		}
	}

	// Handle boost percentage input
	void handleBoostPercentInput(CreatureObject* creature, SuiBox* sui, const String& input) {
		float boostPercent = 0.0f;
		
		try {
			boostPercent = Float::valueOf(input);
		} catch (...) {
			creature->sendSystemMessage("Invalid boost percentage. Please enter a number.");
			handleBack(creature);
			return;
		}

		// Validate minimum 0.1%
		if (boostPercent < 0.1f) {
			creature->sendSystemMessage("Minimum boost is 0.1%");
			handleBack(creature);
			return;
		}

		// Get stored values
		String resourceType = sui->getDataObject()->getString("selectedResourceType");
		String statToBoost = sui->getDataObject()->getString("selectedStat");
		String planet = sui->getDataObject()->getString("selectedPlanet");

		// Calculate boosted value (historical best * (1 + boost/100))
		// For now, use placeholder historical best of 980
		int historicalBest = 980; // Would query ResourceManager for actual historical best
		float multiplier = 1.0f + (boostPercent / 100.0f);
		int boostedValue = (int)((float)historicalBest * multiplier + 0.5f);

		creature->sendSystemMessage("Spawning enhanced " + resourceType + " with " + statToBoost + " boosted to " + String::valueOf(boostedValue));
		creature->sendSystemMessage("Boost: " + String::valueOf(boostPercent) + "% (Historical best: " + String::valueOf(historicalBest) + " -> New: " + String::valueOf(boostedValue) + ")");

		// In production, would call ResourceManager to spawn with custom stats
		// For now, spawn normal resource as placeholder
		ZoneServer* zoneServer = creature->getZoneServer();
		if (zoneServer != nullptr) {
			ResourceManager* resMan = zoneServer->getResourceManager();
			if (resMan != nullptr) {
				ManagedReference<ResourceSpawn*> newSpawn = resMan->spawnResource(resourceType, planet, 86400 * 7);
				if (newSpawn != nullptr) {
					creature->sendSystemMessage("Spawned: " + newSpawn->getName());
				}
			}
		}

		handleBack(creature);
	}

	// ============== UI DISPLAY FUNCTIONS ==============

	void showPlanetResources(CreatureObject* creature, const String& planet) const {
		ZoneServer* zoneServer = creature->getZoneServer();
		if (zoneServer == nullptr)
			return;

		ResourceManager* resMan = zoneServer->getResourceManager();
		if (resMan == nullptr)
			return;

		ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
		sui->setPromptTitle("Resources: " + planet);
		sui->setPromptText("Select a resource to manage");
		sui->getDataObject()->setString("selectedPlanet", planet);
		sui->getDataObject()->setInt("toolState", STATE_SELECT_PLANET);

		// Get resources for this planet by survey type (3 = all types)
		Vector<ManagedReference<ResourceSpawn*>> resources;
		resMan->getResourceListByType(resources, 3, planet);

		uint64 currentTime = System::getTime();
		int count = 0;

		for (int i = 0; i < resources.size(); i++) {
			ManagedReference<ResourceSpawn*> spawn = resources.get(i);
			if (spawn == nullptr)
				continue;

			String resourceName = spawn->getName();
			String resourceClass = spawn->getFinalClass();
			
			uint64 despawnTime = spawn->getDespawned();
			String timeStr = "Unknown";
			
			if (despawnTime > currentTime) {
				int timeRemaining = despawnTime - currentTime;
				int days = timeRemaining / 86400;
				int hours = (timeRemaining % 86400) / 3600;
				int mins = (timeRemaining % 3600) / 60;
				timeStr = String::valueOf(days) + "d " + String::valueOf(hours) + "h";
			} else {
				timeStr = "EXPIRED";
			}

			// Build display string
			StringBuffer display;
			display << resourceName << " [" << resourceClass << "] - " << timeStr;
			
			// Add key stats
			int oq = spawn->getValueOf("res_quality");
			int cd = spawn->getValueOf("res_conductivity");
			int dr = spawn->getValueOf("res_decay_resist");
			
			display << " | OQ:" << oq << " CD:" << cd << " DR:" << dr;

			sui->addMenuItem(display.toString());
			count++;
		}

		if (count == 0) {
			sui->addMenuItem("No active resources on this planet");
		}

		sui->setCallback(new AdminResourceToolSuiCallback(server));
		sui->setOkButton(true, "Select");
		sui->setCancelButton(true, "Back");
		
		creature->sendMessage(sui->generateMessage());
	}

	void showResourceHistory(CreatureObject* creature) const {
		ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
		sui->setPromptTitle("Resource History");
		sui->setPromptText("Historical record of spawned resources");
		
		// Show placeholder - in production would query ResourceManager's historical tracking
		sui->addMenuItem("=== RECENTLY SPAWNED ===");
		sui->addMenuItem("History tracking requires additional implementation");
		sui->addMenuItem("");
		sui->addMenuItem("Note: This feature shows resources that have");
		sui->addMenuItem("spawned since server start, including those");
		sui->addMenuItem("that have already despawned.");
		
		sui->setCallback(new AdminResourceToolSuiCallback(server));
		sui->setOkButton(true, "OK");
		sui->setCancelButton(true, "Back");
		
		creature->sendMessage(sui->generateMessage());
	}

	void showSpawnNewResource(CreatureObject* creature) const {
		ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
		sui->setPromptTitle("Spawn New Resource");
		sui->setPromptText("Select a resource type to spawn");
		sui->getDataObject()->setInt("toolState", STATE_SELECT_RESOURCE_TYPE);
		
		// Add resource types
		sui->addMenuItem("ore");
		sui->addMenuItem("metal");
		sui->addMenuItem("energy");
		sui->addMenuItem("gas");
		sui->addMenuItem("flora");
		sui->addMenuItem("organic");
		sui->addMenuItem("water");
		sui->addMenuItem("hide");
		sui->addMenuItem("bone");
		sui->addMenuItem("meat");
		sui->addMenuItem("dairy");
		sui->addMenuItem("grain");
		sui->addMenuItem("vegetable");
		sui->addMenuItem("fruit");
		sui->addMenuItem("spice");
		
		sui->setCallback(new AdminResourceToolSuiCallback(server));
		sui->setOkButton(true, "Select");
		sui->setCancelButton(true, "Back");
		
		creature->sendMessage(sui->generateMessage());
	}

	void showSpawnEnhancedResource(CreatureObject* creature) const {
		ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
		sui->setPromptTitle("Step 1: Select Resource Type");
		sui->setPromptText("Select a resource type to spawn with enhanced stats");
		sui->getDataObject()->setInt("toolState", STATE_SELECT_RESOURCE_TYPE);
		
		// Add resource types (reuse spawn new flow)
		sui->addMenuItem("ore");
		sui->addMenuItem("metal");
		sui->addMenuItem("energy");
		sui->addMenuItem("gas");
		sui->addMenuItem("flora");
		sui->addMenuItem("organic");
		sui->addMenuItem("water");
		sui->addMenuItem("hide");
		
		sui->setCallback(new AdminResourceToolSuiCallback(server));
		sui->setOkButton(true, "Next");
		sui->setCancelButton(true, "Back");
		
		creature->sendMessage(sui->generateMessage());
	}

	void showGiveResourceMenu(CreatureObject* creature) const {
		ZoneServer* zoneServer = creature->getZoneServer();
		if (zoneServer == nullptr)
			return;

		ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
		sui->setPromptTitle("Give Resource to Player");
		sui->setPromptText("This feature allows giving resources directly to players.");
		sui->getDataObject()->setInt("toolState", STATE_SELECT_GIVE_RESOURCE);
		
		sui->addMenuItem("=== Select Resource to Give ===");
		
		// Get some active resources for selection
		ResourceManager* resMan = zoneServer->getResourceManager();
		if (resMan != nullptr) {
			Vector<ManagedReference<ResourceSpawn*>> resources;
			resMan->getResourceListByType(resources, 3, "tatooine");
			
			for (int i = 0; i < resources.size() && i < 20; i++) {
				ManagedReference<ResourceSpawn*> spawn = resources.get(i);
				if (spawn != nullptr) {
					sui->addMenuItem(spawn->getName());
				}
			}
		}
		
		sui->addMenuItem("Custom Resource Name...");
		
		sui->setCallback(new AdminResourceToolSuiCallback(server));
		sui->setOkButton(true, "Select");
		sui->setCancelButton(true, "Back");
		
		creature->sendMessage(sui->generateMessage());
	}
};

#endif /*ADMINRESOURCETOOLSUICALLBACK_H_*/
