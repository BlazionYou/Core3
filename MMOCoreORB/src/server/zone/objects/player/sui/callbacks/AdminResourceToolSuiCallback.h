/*
				Copyright <BlazinYou>
		See file COPYING for copying conditions.*/

#ifndef ADMINRESOURCETOOLSUICALLBACK_H_
#define ADMINRESOURCETOOLSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"

class AdminResourceToolSuiCallback : public SuiCallback {

public:
	AdminResourceToolSuiCallback(ZoneServer* serv) : SuiCallback(serv) {

	}

	void run(CreatureObject* creature, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);
		
		if (!sui->isListBox() || cancelPressed || server == nullptr)
			return;

		if (!creature->isPlayerCreature())
			return;

		SuiListBox* listBox = cast<SuiListBox*>(sui);
		int index = Integer::valueOf(args->get(0).toString());

		if (index < 0)
			return;

		String selection = listBox->getMenuItemName(index);

		// Handle different selections
		if (selection.contains("Planet:")) {
			String planet = selection.replaceFirst("Planet: ", "");
			showResourcesForPlanet(creature, planet);
		} else if (selection == "View History") {
			showResourceHistory(creature);
		} else if (selection == "Spawn New Resource") {
			creature->sendSystemMessage("Use /gmCreateSpecificResource or /gmCreateClassResource commands");
		} else if (selection == "Spawn Enhanced Resource") {
			creature->sendSystemMessage("Enhanced spawn - Coming soon");
		} else if (selection == "Give Resource to Player") {
			creature->sendSystemMessage("Give resources via survey tools or containers");
		}
	}

private:
	void showResourcesForPlanet(CreatureObject* creature, const String& planet) const {
		creature->sendSystemMessage("Viewing resources on " + planet + " - Use survey tools to see active resources");
	}

	void showResourceHistory(CreatureObject* creature) const {
		creature->sendSystemMessage("Resource history - Coming soon");
	}
};

#endif /*ADMINRESOURCETOOLSUICALLBACK_H_*/
