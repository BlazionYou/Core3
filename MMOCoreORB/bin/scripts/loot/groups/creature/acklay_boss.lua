-- Acklay Boss Loot Group
-- Copy of acklay with boss drop rates

acklay_boss = {
	description = "",
	minimumLevel = 0,
	maximumLevel = 0,
	lootItems = {
		{itemTemplate = "acklay_bones_rare", weight = 10000000}
	}
}

addLootGroupTemplate("acklay_boss", acklay_boss)
