-- Fire Spider Boss Loot Group
-- Copy of fire_breathing_spider with boss drop rates

fire_spider_boss = {
	description = "",
	minimumLevel = 0,
	maximumLevel = 0,
	lootItems = {
		{itemTemplate = "geo_spider_fang", weight = 2500000},
		{itemTemplate = "geo_spider_venom_rare", weight = 7500000}
	}
}

addLootGroupTemplate("fire_spider_boss", fire_spider_boss)
