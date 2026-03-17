/*
		Copyright <BlazinYou>
	See file COPYING for copying conditions.*/

#ifndef ADMINRESOURCETOOLCOMMAND_H_
#define ADMINRESOURCETOOLCOMMAND_H_

#include "server/zone/objects/creature/commands/QueueCommand.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"

class AdminResourceToolCommand : public QueueCommand {
public:

	AdminResourceToolCommand(const String& name, ZoneProcessServer* serv) : QueueCommand(name, serv) {
		setCharacterAbility("admin");
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const;

private:
	void showMainMenu(CreatureObject* creature, ZoneServer* zoneServer) const;
	void showPlanetSelection(CreatureObject* creature, ZoneServer* zoneServer) const;
	void showResourcesForPlanet(CreatureObject* creature, ZoneServer* zoneServer, const String& planet) const;
	void showResourceHistory(CreatureObject* creature, ZoneServer* zoneServer) const;
	void showSpawnMenu(CreatureObject* creature, ZoneServer* zoneServer) const;
	void showGiveResourceMenu(CreatureObject* creature, ZoneServer* zoneServer) const;
	void despawnResource(CreatureObject* creature, ZoneServer* zoneServer, const String& resourceName) const;
	void showHelp(CreatureObject* creature) const;
};

#endif //ADMINRESOURCETOOLCOMMAND_H_
