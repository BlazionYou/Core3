# Planetary Militia Campaign - Complete Implementation Guide

## Overview

This is a **completely standalone quest system** that adds extensive content to each planet without interfering with ThemePark, Jedi unlocks, or any existing game mechanics. It uses a completely separate state tracking namespace (`pmc_` prefix) to ensure zero conflicts.

### System Flow

```
1. Player talks to ANY static NPC in a city
   ↓
2. NPC directs player to City Militia Commander
   ↓
3. Player completes 6 missions (one of each type):
   - Deliver
   - Hunt
   - Escort
   - Assassinate
   - Confiscate
   - Destroy
   ↓
4. All 6 complete = City Complete
   ↓
5. All cities on planet complete = Planet Quest Unlocked
   ↓
6. Planet Quest: Visit POIs to prepare troops
   ↓
7. Final Boss Fight with reinforcements
   ↓
8. Boss drops: Custom loot item OR crafting schematic
```

---

## Directory Structure

All files for this system should be created in:

```
Core3/MMOCoreORB/bin/scripts/screenplays/
├── planetary/
│   ├── base/
│   │   └── cityQuest.lua          # Base class (reuse for all planets)
│   ├── rori/
│   │   ├── rori.lua              # Main planet screenplay
│   │   ├── cities.lua            # City definitions + missions
│   │   ├── planetQuest.lua       # Planet-wide quest + boss
│   │   └── conversations/
│   │       ├── narmle_commander_conv_handler.lua
│   │       ├── restuss_commander_conv_handler.lua
│   │       ├── rebel_commander_conv_handler.lua
│   │       ├── citizen_conv_handler.lua  # Generic entry point
│   │       └── ...
│   ├── naboo/
│   │   └── ... (same structure)
│   ├── corellia/
│   │   └── ... (same structure)
│   └── (other planets as needed)
└── PLANETARY_ADVENTURES.md       # This file
```

---

## State Tracking

All state uses the prefix `pmc_` (Planetary Militia Campaign) to avoid conflicts:

```lua
-- Per-City State (one set per city per player):
playerID .. ":pmc_city_started:" .. planet .. ":" .. cityName     -- 1 = started
playerID .. ":pmc_city_mission:" .. planet .. ":" .. cityName .. ":" .. missionNum  -- 1 = complete
playerID .. ":pmc_city_complete:" .. planet .. ":" .. cityName    -- 1 = all 6 done

-- Planet State:
playerID .. ":pmc_planet_unlocked:" .. planet                      -- 1 = can start planet quest
playerID .. ":pmc_poi_complete:" .. planet .. ":" .. poiName      -- 1 = POI visited
playerID .. ":pmc_boss_summoned:" .. planet                       -- 1 = boss spawned
playerID .. ":pmc_planet_complete:" .. planet                      -- 1 = planet done
```

---

## Mission Types Implementation

Each mission type requires different handling. Here's how to implement each:

### 1. Deliver Mission
```lua
-- Player must deliver item from NPC A to NPC B
-- Create active area at destination
-- On player enter: spawn destination NPC, trigger completion
```

### 2. Hunt Mission
```lua
-- Player must kill X enemies of specific type
-- Use KILLEDCREATURE observer
-- Track kill count, complete when reached
```

### 3. Escort Mission
```lua
-- NPC follows player to destination
-- Use AiAgent with AI_FOLLOW flag
-- Spawn dropoff active area
-- On enter: complete mission, NPC gives reward
```

### 4. Assassinate Mission
```lua
-- Spawn target NPC at location
-- Use OBJECTDESTRUCTION observer
-- On target death: complete mission
```

### 5. Confiscate Mission
```lua
-- Kill target, loot specific item
-- Use ITEMLOOTED observer
-- On item looted: complete mission
```

### 6. Destroy Mission
```lua
-- Spawn building + defenders
-- Use OBJECTDESTRUCTION observer on building
-- On building destroyed: complete mission
```

---

## Base Class: cityQuest.lua

This is the reusable base class for all city quests:

```lua
-- Core3/MMOCoreORB/bin/scripts/screenplays/planetary/base/cityQuest.lua

local ObjectManager = require("managers.object.object_manager")
local Logger = require("utils.logger")

CityQuest = ScreenPlay:new {
    screenplayName = "",
    planetName = "",
    cityName = "",
    
    -- Screenplay state prefix
    statePrefix = "pmc_",
    
    -- Number of missions per city
    missionsPerCity = 6,
    
    -- Mission types in order
    missionTypes = {"deliver", "hunt", "escort", "assassinate", "confiscate", "destroy"},
}

-- Check if player has started city quest
function CityQuest:hasStartedCity(pPlayer)
    if pPlayer == nil then return false end
    local state = self.statePrefix .. "city_started:" .. self.planetName .. ":" .. self.cityName
    return readData(SceneObject(pPlayer):getObjectID() .. state) == 1
end

-- Start city quest for player
function CityQuest:startCity(pPlayer)
    if pPlayer == nil then return false end
    local playerID = SceneObject(pPlayer):getObjectID()
    local state = self.statePrefix .. "city_started:" .. self.planetName .. ":" .. self.cityName
    writeData(playerID .. state, 1)
    return true
end

-- Check if specific mission is complete
function CityQuest:isMissionComplete(pPlayer, missionNum)
    if pPlayer == nil then return false end
    local playerID = SceneObject(pPlayer):getObjectID()
    local state = self.statePrefix .. "city_mission:" .. self.planetName .. ":" .. self.cityName .. ":" .. missionNum
    return readData(playerID .. state) == 1
end

-- Mark mission complete
function CityQuest:completeMission(pPlayer, missionNum)
    if pPlayer == nil then return false end
    local playerID = SceneObject(pPlayer):getObjectID()
    local state = self.statePrefix .. "city_mission:" .. self.planetName .. ":" .. self.cityName .. ":" .. missionNum
    writeData(playerID .. state, 1)
    Logger:log("Mission " .. missionNum .. " completed for " .. self.cityName, LT_INFO)
    
    -- Check if all missions complete
    self:checkCityCompletion(pPlayer)
    return true
end

-- Check if all missions for city are done
function CityQuest:checkCityCompletion(pPlayer)
    if pPlayer == nil then return false end
    
    local allComplete = true
    for i = 1, self.missionsPerCity do
        if not self:isMissionComplete(pPlayer, i) then
            allComplete = false
            break
        end
    end
    
    if allComplete then
        local playerID = SceneObject(pPlayer):getObjectID()
        local state = self.statePrefix .. "city_complete:" .. self.planetName .. ":" .. self.cityName
        writeData(playerID .. state, 1)
        CreatureObject(pPlayer):sendSystemMessage("@pmc:city_complete") -- You have completed all missions for [city]!
        
        -- Check if all cities on planet complete
        self:checkPlanetUnlocked(pPlayer)
    end
    
    return allComplete
end

-- Get current mission number (first incomplete)
function CityQuest:getCurrentMission(pPlayer)
    if pPlayer == nil then return 1 end
    for i = 1, self.missionsPerCity do
        if not self:isMissionComplete(pPlayer, i) then
            return i
        end
    end
    return self.missionsPerCity + 1 -- All complete
end

-- Check if planet quest should unlock (implement in planet-specific file)
function CityQuest:checkPlanetUnlocked(pPlayer)
    -- Override in planet screenplay
end

return CityQuest
```

---

## Planet Main Screenplay: rori.lua

```lua
-- Core3/MMOCoreORB/bin/scripts/screenplays/planetary/rori/rori.lua

require("screenplays.screenplay")
require("screenplays.planetary.base.cityQuest")
local Logger = require("utils.logger")

RoriPlanetaryQuest = ScreenPlay:new {
    screenplayName = "RoriPlanetaryQuest",
    numberOfActs = 1,
}

registerScreenPlay("RoriPlanetaryQuest", true)

function RoriPlanetaryQuest:start()
    if isZoneEnabled("rori") then
        -- Spawn any static elements if needed
        -- Most content is handled via conversation triggers
    end
end

-- Check if all cities on Rori are complete
function RoriPlanetaryQuest:checkAllCitiesComplete(pPlayer)
    if pPlayer == nil then return false end
    
    local cities = {"narmle", "restuss", "rebel_outpost"}
    local playerID = SceneObject(pPlayer):getObjectID()
    
    for _, city in ipairs(cities) do
        local state = "pmc_city_complete:rori:" .. city
        if readData(playerID .. state) ~= 1 then
            return false
        end
    end
    
    -- All cities complete - unlock planet quest
    writeData(playerID .. ":pmc_planet_unlocked:rori", 1)
    CreatureObject(pPlayer):sendSystemMessage("@pmc:planet_unlocked") -- The planet assault is ready!
    return true
end

-- Get cities for Rori
function RoriPlanetaryQuest:getCities()
    return {
        {
            name = "narmle",
            displayName = "Narmle",
            mainGiver = "narmle_militia_commander",
            spawnX = -5456.4,
            spawnZ = 80.0,
            spawnY = -2095.6,
            cellID = 0,
            stfFile = "pmc/narmle",
            missions = {
                {
                    type = "deliver",
                    targetTemplate = "spice_collective_courier",
                    targetLocation = {x = 5514.9, z = 77.3, y = 1009.3},
                    description = "deliver_supplies",
                    rewardCredits = 500
                },
                {
                    type = "hunt",
                    targetTemplate = "gundark_rogue",
                    killCount = 5,
                    spawnLocation = {x = -4487, z = 75, y = 1087},
                    description = "hunt_gundarks",
                    rewardCredits = 750
                },
                {
                    type = "escort",
                    targetTemplate = "commoner_naboo",
                    destinationLocation = {x = -5456, z = 80, y = -2095},
                    description = "escort_civilian",
                    rewardCredits = 600
                },
                {
                    type = "assassinate",
                    targetTemplate = "kobola_smuggler",
                    spawnLocation = {x = 987, z = 107, y = 7560},
                    description = "eliminate_smuggler",
                    rewardCredits = 1000
                },
                {
                    type = "confiscate",
                    targetTemplate = "gundark_thief",
                    itemTemplate = "object/tangible/loot/misc/stolen_credits.iff",
                    spawnLocation = {x = -3649, z = 79, y = 5528},
                    description = "retrieve_stolen",
                    rewardCredits = 800
                },
                {
                    type = "destroy",
                    buildingTemplate = "object/building/general/camp_large.iff",
                    spawnLocation = {x = -5286, z = 80, y = -1915},
                    defenders = {"scorched_krevol", "scorched_krevol"},
                    description = "destroy_camp",
                    rewardCredits = 1200
                }
            }
        },
        {
            name = "restuss",
            displayName = "Restuss",
            mainGiver = "rebel_recruiter",
            spawnX = -5000,
            spawnZ = 80,
            spawnY = -3500,
            cellID = 0,
            stfFile = "pmc/restuss",
            missions = {-- Similar structure }
        },
        {
            name = "rebel_outpost",
            displayName = "Rebel Outpost",
            mainGiver = "rebel_major",
            spawnX = -5300,
            spawnZ = 80,
            spawnY = 4500,
            cellID = 0,
            stfFile = "pmc/rebel_outpost",
            missions = {-- Similar structure }
        }
    }
end

return RoriPlanetaryQuest
```

---

## City Configuration: cities.lua

```lua
-- Core3/MMOCoreORB/bin/scripts/screenplays/planetary/rori/cities.lua

-- This file defines all cities and their missions for the planet
-- Can be generated programmatically or defined manually

RoriCities = {
    {
        -- City 1: Narmle
        name = "narmle",
        displayName = "Narmle",
        description = "A small settlement fighting against local threats",
        
        -- Main quest giver NPC
        mainGiver = {
            template = "narmle_militia_commander",
            x = -5456.4,
            z = 80.0,
            y = -2095.6,
            cellID = 0,
            convTemplate = "NarmleCommanderConvTemplate"
        },
        
        -- Entry point NPCs (static NPCs that redirect to main giver)
        entryPoints = {
            {template = "commoner_fat", convTemplate = "CitizenConvTemplate"},
            {template = "bounty_hunter", convTemplate = "CitizenConvTemplate"},
            {template = "fringer", convTemplate = "CitizenConvTemplate"},
            {template = "citizen", convTemplate = "CitizenConvTemplate"},
        },
        
        -- Mission definitions (6 total, one of each type)
        missions = {
            -- Mission 1: DELIVER
            {
                id = 1,
                type = "deliver",
                stfName = "deliver_supplies",
                description = "Deliver military supplies to the Spice Collective Courier at the northern camp",
                targetNPC = "spice_collective_courier",
                spawnPlanet = "rori",
                spawnX = 5514.9,
                spawnZ = 77.3,
                spawnY = 1009.3,
                reward = {
                    type = "credits",
                    amount = 500
                },
                nextMission = 2
            },
            
            -- Mission 2: HUNT
            {
                id = 2,
                type = "hunt",
                stfName = "hunt_gundarks",
                description = "Eliminate 5 Gundark Rogues threatening the northern debris field",
                enemyTemplate = "gundark_rogue",
                killCount = 5,
                spawnPlanet = "rori",
                spawnX = -4487,
                spawnZ = 75,
                spawnY = 1087,
                spawnRadius = 50,
                reward = {
                    type = "credits",
                    amount = 750
                },
                nextMission = 3
            },
            
            -- Mission 3: ESCORT
            {
                id = 3,
                type = "escort",
                stfName = "escort_civilian",
                description = "Escort the civilian to the militia headquarters in Narmle",
                escortNPC = "commoner_naboo",
                spawnPlanet = "rori",
                spawnX = -1713,
                spawnZ = 75,
                spawnY = 6144,
                destinationX = -5456,
                destinationZ = 80,
                destinationY = -2095,
                reward = {
                    type = "credits",
                    amount = 600
                },
                nextMission = 4
            },
            
            -- Mission 4: ASSASSINATE
            {
                id = 4,
                type = "assassinate",
                stfName = "eliminate_smuggler",
                description = "Find and eliminate the Kobola Smuggler at the crash site",
                targetNPC = "kobola_smuggler",
                spawnPlanet = "rori",
                spawnX = 987,
                spawnZ = 107,
                spawnY = 7560,
                reward = {
                    type = "credits",
                    amount = 1000
                },
                nextMission = 5
            },
            
            -- Mission 5: CONFISCATE
            {
                id = 5,
                type = "confiscate",
                stfName = "retrieve_stolen",
                description = "Defeat the Bounty Hunter and retrieve the stolen military intelligence",
                targetNPC = "bounty_hunter_thug",
                itemTemplate = "object/tangible/loot/misc/encrypted_disk.iff",
                itemName = "Encrypted Data Disk",
                spawnPlanet = "rori",
                spawnX = -3649,
                spawnZ = 79,
                spawnY = 5528,
                reward = {
                    type = "credits",
                    amount = 800
                },
                nextMission = 6
            },
            
            -- Mission 6: DESTROY
            {
                id = 6,
                type = "destroy",
                stfName = "destroy_camp",
                description = "Destroy the Gundark encampment near the city",
                buildingTemplate = "object/building/general/camp_large.iff",
                spawnPlanet = "rori",
                spawnX = -5286,
                spawnZ = 80,
                spawnY = -1915,
                defenders = {
                    {template = "scorched_krevol", count = 3},
                    {template = "scorched_krevol", count = 3}
                },
                reward = {
                    type = "credits",
                    amount = 1200
                },
                nextMission = nil -- City complete
            }
        }
    },
    
    -- City 2: Restuss (similar structure)
    {
        name = "restuss",
        displayName = "Restuss",
        -- ... same structure
    },
    
    -- City 3: Rebel Outpost (similar structure)
    {
        name = "rebel_outpost",
        displayName = "Rebel Outpost",
        -- ... same structure
    }
}

return RoriCities
```

---

## Conversation Handler: Main Giver

```lua
-- Core3/MMOCoreORB/bin/scripts/screenplays/planetary/rori/conversations/narmle_commander_conv_handler.lua

require("screenplays.planetary.base.cityQuest")

NarmleCommanderConvoHandler = conv_handler:new {}

function NarmleCommanderConvoHandler:getInitialScreen(pPlayer, pNpc, pConvTemplate)
    -- Check player's current state
    local playerID = SceneObject(pPlayer):getObjectID()
    local stateKey = "pmc_city_complete:rori:narmle"
    local isComplete = readData(playerID .. stateKey) == 1
    
    if isComplete then
        return pConvTemplate:getScreen("city_complete")
    end
    
    -- Check if city started
    local startedKey = "pmc_city_started:rori:narmle"
    local isStarted = readData(playerID .. startedKey) == 1
    
    if isStarted then
        -- Get current mission
        local mission1 = readData(playerID .. "pmc_city_mission:rori:narmle:1") == 1
        local mission2 = readData(playerID .. "pmc_city_mission:rori:narmle:2") == 1
        local mission3 = readData(playerID .. "pmc_city_mission:rori:narmle:3") == 1
        local mission4 = readData(playerID .. "pmc_city_mission:rori:narmle:4") == 1
        local mission5 = readData(playerID .. "pmc_city_mission:rori:narmle:5") == 1
        
        if not mission1 then return pConvTemplate:getScreen("mission_1") end
        if not mission2 then return pConvTemplate:getScreen("mission_2") end
        if not mission3 then return pConvTemplate:getScreen("mission_3") end
        if not mission4 then return pConvTemplate:getScreen("mission_4") end
        if not mission5 then return pConvTemplate:getScreen("mission_5") end
        return pConvTemplate:getScreen("mission_6")
    end
    
    -- New player
    return pConvTemplate:getScreen("start")
end

function NarmleCommanderConvoHandler:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
    local screen = LuaConversationScreen(pConvScreen)
    local optionID = screen:getOptionID(selectedOption)
    local playerID = SceneObject(pPlayer):getObjectID()
    
    -- Handle screen transitions
    if screen:getScreenID() == "start" then
        if optionID == 1 then -- Accept quest
            writeData(playerID .. ":pmc_city_started:rori:narmle", 1)
            return pConvTemplate:getScreen("mission_1")
        end
    elseif screen:getScreenID() == "mission_1" then
        if optionID == 1 then -- Accept mission
            -- Start deliver mission here
            self:startDeliverMission(pPlayer, 1)
        end
    elseif screen:getScreenID() == "mission_2" then
        if optionID == 1 then
            self:startHuntMission(pPlayer, 2)
        end
    end
    -- ... handle other missions
    
    return pConvScreen
end

function NarmleCommanderConvoHandler:startDeliverMission(pPlayer, missionNum)
    local playerID = SceneObject(pPlayer):getObjectID()
    local ghost = CreatureObject(pPlayer):getPlayerObject()
    
    -- Spawn target NPC
    local pTarget = spawnMobile("rori", "spice_collective_courier", 0, 5514.9, 77.3, 1009.3, 0, 0)
    
    if pTarget ~= nil then
        -- Create active area at destination
        local pArea = spawnActiveArea("rori", "object/active_area.iff", 5514.9, 77.3, 1009.3, 20, 0)
        if pArea ~= nil then
            writeData(SceneObject(pArea):getObjectID() .. ":ownerID", playerID)
            createObserver(ENTEREDAREA, "RoriPlanetaryQuest", "onDeliverDestination", pArea)
        end
        
        -- Add waypoint
        PlayerObject(ghost):addWaypoint("rori", "Deliver Supplies", "Deliver to Spice Collective Courier", 5514.9, 0, 1009.3, WAYPOINT_ORANGE, true, true, 0)
    end
end

function NarmleCommanderConvoHandler:startHuntMission(pPlayer, missionNum)
    local playerID = SceneObject(pPlayer):getObjectID()
    local ghost = CreatureObject(pPlayer):getPlayerObject()
    
    -- Spawn enemies
    for i = 1, 5 do
        local x = -4487 + (math.random() * 100 - 50)
        local y = 1087 + (math.random() * 100 - 50)
        local pEnemy = spawnMobile("rori", "gundark_rogue", 0, x, 75, y, 0, 0)
        if pEnemy ~= nil then
            writeData(SceneObject(pEnemy):getObjectID() .. ":missionOwnerID", playerID)
            createObserver(OBJECTDESTRUCTION, "RoriPlanetaryQuest", "onEnemyKilled", pEnemy)
        end
    end
    
    -- Add waypoint
    PlayerObject(ghost):addWaypoint("rori", "Hunt Gundarks", "Kill 5 Gundark Rogues", -4487, 0, 1087, WAYPOINT_ORANGE, true, true, 0)
end

-- Continue similar handlers for other mission types...

return NarmleCommanderConvoHandler
```

---

## Entry Point Conversation: Citizen Redirect

```lua
-- Core3/MMOCoreORB/bin/scripts/screenplays/planetary/rori/conversations/citizen_conv_handler.lua

-- Generic conversation for any static NPC that redirects to city main giver
-- This allows players to start the quest from ANY static NPC

CitizenPlanetaryConvoHandler = conv_handler:new {}

function CitizenPlanetaryConvoHandler:getInitialScreen(pPlayer, pNpc, pConvTemplate)
    local playerID = SceneObject(pPlayer):getObjectID()
    
    -- Check if player has any PMC progress on this planet
    local planetState = readData(playerID .. ":pmc_planet_unlocked:rori")
    
    if planetState == 1 then
        return pConvTemplate:getScreen("planet_unlocked")
    end
    
    -- Check if any city complete
    local narmle = readData(playerID .. ":pmc_city_complete:rori:narmle") == 1
    local restuss = readData(playerID .. ":pmc_city_complete:rori:restuss") == 1
    local rebel = readData(playerID .. ":pmc_city_complete:rori:rebel_outpost") == 1
    
    if narmle and restuss and rebel then
        return pConvTemplate:getScreen("all_cities_complete")
    end
    
    -- Check current city progress
    if readData(playerID .. ":pmc_city_started:rori:narmle") == 1 then
        return pConvTemplate:getScreen("talk_narmle")
    elseif readData(playerID .. ":pmc_city_started:rori:restuss") == 1 then
        return pConvTemplate:getScreen("talk_restuss")
    elseif readData(playerID .. ":pmc_city_started:rori:rebel_outpost") == 1 then
        return pConvTemplate:getScreen("talk_rebel")
    end
    
    -- New player - direct to nearest city
    return pConvTemplate:getScreen("new_quest")
end

function CitizenPlanetaryConvoHandler:runScreenHandlers(pConvTemplate, pPlayer, pNpc, selectedOption, pConvScreen)
    local screen = LuaConversationScreen(pConvScreen)
    local optionID = screen:getOptionID(selectedOption)
    
    if screen:getScreenID() == "new_quest" then
        if optionID == 1 then -- Go to Narmle
            -- Add waypoint to Narmle
            local ghost = CreatureObject(pPlayer):getPlayerObject()
            PlayerObject(ghost):addWaypoint("rori", "Narmle Militia", "Talk to the Militia Commander", -5456, 0, -2095, WAYPOINT_GREEN, true, true, 0)
        elseif optionID == 2 then -- Go to Restuss
            -- Add waypoint to Restuss
        elseif optionID == 3 then -- Go to Rebel Outpost
            -- Add waypoint to Rebel Outpost
        end
    end
    
    return pConvScreen
end

return CitizenPlanetaryConvoHandler
```

---

## Planet Quest: planetQuest.lua

```lua
-- Core3/MMOCoreORB/bin/scripts/screenplays/planetary/rori/planetQuest.lua

-- This handles the planet-wide quest after all cities are complete
-- Phase 1: Visit POIs to prepare troops
-- Phase 2: Summon and defeat the world boss

require("screenplays.screenplay")

RoriPlanetQuest = ScreenPlay:new {
    screenplayName = "RoriPlanetQuest",
    planetName = "rori",
}

registerScreenPlay("RoriPlanetQuest", true)

function RoriPlanetQuest:start()
    -- Nothing to spawn at server start
end

-- POIs to visit for planet quest
function RoriPlanetQuest:getPOIs()
    return {
        {
            name = "narmle_territory",
            displayName = "Narmle Territory",
            x = -5000,
            z = 80,
            y = -2000,
            description = "Coordinate with Narmle militia",
            requiredState = "pmc_city_complete:rori:narmle"
        },
        {
            name = "restuss_ruins",
            displayName = "Restuss Ruins",
            x = -5000,
            z = 80,
            y = -3500,
            description = "Coordinate with Restuss rebels",
            requiredState = "pmc_city_complete:rori:restuss"
        },
        {
            name = "rebel_base",
            displayName = "Rebel Outpost",
            x = -5300,
            z = 80,
            y = 4500,
            description = "Coordinate with Rebel command",
            requiredState = "pmc_city_complete:rori:rebel_outpost"
        }
    }
end

-- Check if player has completed all POIs
function RoriPlanetQuest:checkPOIsComplete(pPlayer)
    if pPlayer == nil then return false end
    
    local pois = self:getPOIs()
    local playerID = SceneObject(pPlayer):getObjectID()
    
    for _, poi in ipairs(pois) do
        if readData(playerID .. ":pmc_poi_complete:rori:" .. poi.name) ~= 1 then
            return false
        end
    end
    
    return true
end

-- Summon the world boss
function RoriPlanetQuest:summonBoss(pPlayer)
    if pPlayer == nil then return false end
    
    local playerID = SceneObject(pPlayer):getObjectID()
    
    -- Check if already summoned
    if readData(playerID .. ":pmc_boss_summoned:rori") == 1 then
        return false
    end
    
    -- Boss location: Gundark King at debris field
    local bossX = -4493
    local bossZ = 75
    local bossY = 1085
    
    -- Spawn boss
    local pBoss = spawnMobile("rori", "gundark_king", 1, bossX, bossZ, bossY, 0, 0)
    
    if pBoss ~= nil then
        -- Boss stats (make it tough)
        CreatureObject(pBoss):setLevel(80)
        
        -- Spawn reinforcement NPCs
        local reinforcementCount = 4
        for i = 1, reinforcementCount do
            local offsetX = (math.random() * 30) - 15
            local offsetY = (math.random() * 30) - 15
            local pReinforce = spawnMobile("rori", "gundark_elite", 1, 
                bossX + offsetX, bossZ, bossY + offsetY, 0, 0)
            
            if pReinforce ~= nil then
                -- Set reinforce to assist boss
                AiAgent(pReinforce):setDefender(pBoss)
            end
        end
        
        -- Mark boss as summoned
        writeData(playerID .. ":pmc_boss_summoned:rori", 1)
        
        -- Create observer for boss death
        createObserver(OBJECTDESTRUCTION, "RoriPlanetQuest", "onBossKilled", pBoss)
        
        -- Add waypoint
        local ghost = CreatureObject(pPlayer):getPlayerObject()
        PlayerObject(ghost):addWaypoint("rori", "Gundark King", "Defeat the boss and their reinforcements", 
            bossX, bossZ, bossY, WAYPOINT_RED, true, true, 0)
        
        CreatureObject(pPlayer):sendSystemMessage("@pmc:boss_summoned") -- The Gundark King has appeared at the debris field!
        return true
    end
    
    return false
end

-- Handle boss death
function RoriPlanetQuest:onBossKilled(pBoss, pAttacker)
    if pBoss == nil or pAttacker == nil then return 0 end
    
    local playerID = SceneObject(pAttacker):getObjectID()
    
    -- Mark planet complete
    writeData(playerID .. ":pmc_planet_complete:rori", 1)
    
    -- Give reward - custom loot
    local pInventory = CreatureObject(pAttacker):getSlottedObject("inventory")
    if pInventory ~= nil then
        -- Give boss trophy
        local pItem = giveItem(pInventory, "object/tangible/loot/misc/gundark_king_skull.iff", -1)
        
        -- Give crafting schematic (if exists)
        local pSchematic = giveItem(pInventory, "object/tangible/loot/misc/schematic_gundark_weapon.iff", -1)
    end
    
    CreatureObject(pAttacker):sendSystemMessage("@pmc:planet_complete") -- You have completed the Rori campaign!
    
    return 1
end

return RoriPlanetQuest
```

---

## Loading The System

Add to `screenplays.lua`:

```lua
-- Planetary Adventures (PMC - completely separate from ThemePark)
includeFile("planetary/base/cityQuest.lua")
includeFile("planetary/rori/rori.lua")
includeFile("planetary/rori/cities.lua")
includeFile("planetary/rori/planetQuest.lua")
includeFile("planetary/rori/conversations/narmle_commander_conv_handler.lua")
includeFile("planetary/rori/conversations/citizen_conv_handler.lua")
-- ... other conversations
```

---

## NPC Template Setup

After creating the Lua files, you need to set conversation templates on NPCs. This is typically done in the city spawn file:

```lua
-- In rori_narmle.lua (city spawn file)
-- Add this to spawnMobiles() function:

pNpc = spawnMobile("rori", "narmle_militia_commander", 300, -5456.4, 80.0, -2095.6, 45, 0)
if pNpc ~= nil then
    AiAgent(pNpc):setConvoTemplate("NarmleCommanderConvTemplate")
    CreatureObject(pNpc):setOptionBit(INTERESTING)
end
```

---

## Creating Loot Items

### Boss Drop: Gundark King Skull
Create file: `object/tangible/loot/misc/gundark_king_skull.iff`

This is a template file (binary). For testing, use an existing item template and customize via code.

### Crafting Schematic
Create file: `object/tangible/loot/misc/schematic_gundark_weapon.iff`

The schematic should unlock a custom weapon recipe when used.

---

## Replication Guide: Adding New Planets

To add a new planet (e.g., Tatooine):

### 1. Create directory structure
```
planetary/
└── tatooine/
    ├── tatooine.lua
    ├── cities.lua
    ├── planetQuest.lua
    └── conversations/
```

### 2. Copy and modify tatooine.lua
- Change planet name to "tatooine"
- Define city list from tatooine cities

### 3. Define cities in cities.lua
- Use tatooine_static_spawns.lua for NPC templates
- Use tatooine POIs for mission targets

### 4. Create conversation handlers
- Main giver for each city
- Entry point handler

### 5. Add to screenplays.lua
```lua
includeFile("planetary/tatooine/tatooine.lua")
includeFile("planetary/tatooine/cities.lua")
includeFile("planetary/tatooine/planetQuest.lua")
includeFile("planetary/tatooine/conversations/...")
```

### 6. Update city spawn files
- Add convTemplate to appropriate NPCs

---

## Planet City Reference

### Rori (3 Cities)
- Narmle (militia theme)
- Restuss (rebel theme)
- Rebel Outpost (military theme)

### Tatooine (6 Cities)
- Mos Eisley
- Mos Taike
- Mos Entha
- Mos Espa
- Bestine
- Anchorhead

### Naboo (5 Cities)
- Theed
- Keren
- Moenia
- Kaadara
- Deeja Peak

### Corellia (5 Cities)
- Coronet
- Tyrena
- Bela Vistal
- Kor Vella
- Vreni Island

### Talus (2 Cities)
- Nashal
- Dearic

### Dantooine (3 Cities)
- Mining Outpost
- Imperial Outpost
- Agro Outpost

### Dathomir (2 Cities)
- Science Outpost
- Trade Outpost

### Endor (2 Cities)
- Research Outpost
- Smuggler Outpost

### Lok (1 City)
- Nym Stronghold

### Yavin4 (2 Cities)
- Labor Outpost
- Mining Outpost

---

## Best Practices

1. **Test incrementally** - Start with one city, one mission type
2. **Use existing assets** - All NPC templates and items already exist
3. **Keep state separate** - Always use `pmc_` prefix
4. **Reference existing patterns** - Look at generic_businessman.lua for mission structure
5. **Add rewards** - Mix credits, faction standing, and loot

---

## Troubleshooting

### NPCs not responding to conversation
- Check that convTemplate is set on NPC
- Verify conversation file path is correct
- Ensure conversation is loaded in screenplays.lua

### Missions not completing
- Check state keys match exactly (case sensitive)
- Verify observers are created correctly
- Check log files for Lua errors

### Boss won't spawn
- Verify all cities are complete (check state)
- Ensure boss template exists
- Check spawn coordinates are valid

---

## Next Steps

1. Create base class: `planetary/base/cityQuest.lua`
2. Create Rori implementation as template
3. Test with one city, one mission
4. Expand to all mission types
5. Add planet quest
6. Test boss fight
7. Duplicate for other planets
