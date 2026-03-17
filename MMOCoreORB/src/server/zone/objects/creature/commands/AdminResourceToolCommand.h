/*
				Copyright <BlazinYou>
		See file COPYING for copying conditions.*/

#ifndef ADMINRESOURCETOOLCOMMAND_H_
#define ADMINRESOURCETOOLCOMMAND_H_

#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/objects/player/sui/callbacks/AdminResourceToolSuiCallback.h"

class AdminResourceToolCommand : public QueueCommand {
public:

	AdminResourceToolCommand(const String& name, ZoneProcessServer* serv) : QueueCommand(name, serv) {
		setCharacterAbility("admin");
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

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

		// Create the main SUI window
		ManagedReference<SuiListBox*> sui = new SuiListBox(creature, SuiWindowType::ADMIN_RESOURCE_TOOL, SuiListBox::HANDLETWOBUTTON);
		sui->setPromptTitle("Admin Resource Tool");
		sui->setPromptText("Select an option from the menu below.");

		// Add menu items for different sections
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

		// Add callback
		sui->setCallback(new AdminResourceToolSuiCallback(zoneServer));

		// Add buttons
		sui->setOkButton(true, "Select");
		sui->setCancelButton(true, "Close");

		// Send the SUI to the player
		creature->sendMessage(sui->generateMessage());

		return SUCCESS;
	}

};

#endif //ADMINRESOURCETOOLCOMMAND_H_
