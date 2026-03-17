-- Custom Rancor Boss Loot Group
-- 50/50 split between Rancor Hammer and Rancor T21
-- 100% drop rate (chance = 10000000)

rancor_boss = {
	description = "",
	minimumLevel = 0,
	maximumLevel = 0,
	lootItems = {
		{itemTemplate = "rancor_hammer", weight = 5000000},
		{itemTemplate = "rancor_t21", weight = 5000000}
	}
}

addLootGroupTemplate("rancor_boss", rancor_boss)
