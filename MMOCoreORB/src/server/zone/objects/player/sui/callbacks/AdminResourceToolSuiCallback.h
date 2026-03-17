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

	void run(CreatureObject* creature, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args);

private:
	void showMainMenu(CreatureObject* creature, ZoneServer* zoneServer) const;
	void showResourcesForPlanet(CreatureObject* creature, ZoneServer* zoneServer, const String& planet) const;
	void showResourceDetails(CreatureObject* creature, ZoneServer* zoneServer, const String& resourceName) const;
	void showResourceHistory(CreatureObject* creature, ZoneServer* zoneServer) const;
	void showSpawnHelp(CreatureObject* creature, ZoneServer* zoneServer) const;
	void showSpawnEnhancedMenu(CreatureObject* creature, ZoneServer* zoneServer) const;
	void showGiveResourceMenu(CreatureObject* creature, ZoneServer* zoneServer) const;
	void promptDespawnResource(CreatureObject* creature, ZoneServer* zoneServer) const;
};

#endif /*ADMINRESOURCETOOLSUICALLBACK_H_*/
