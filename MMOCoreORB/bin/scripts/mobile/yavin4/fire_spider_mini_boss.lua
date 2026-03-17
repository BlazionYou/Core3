-- Miniature 0.1 Scale Fire Breathing Spider Mini Boss
fire_spider_mini_boss = Creature:new {
	objectName = "@mob/creature_names:geonosian_gaping_spider_fire",
	customName = "Fire Breathing Spider Mini Boss",
	socialGroup = "geonosian_creature",
	mobType = MOB_CARNIVORE,
	faction = "",
	level = 290,
	chanceHit = 0.75,
	damageMin = 1800,
	damageMax = 3200,
	baseXp = 100000,
	baseHAM = 250000,
	baseHAMmax = 300000,
	armor = 2,
	resists = {95,95,-1,95,95,95,-1,-1,-1},
	meatType = "meat_insect",
	meatAmount = 50,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.05,
	ferocity = 95,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	scale = 0.1,
	templates = {"object/mobile/gaping_spider.iff"},
	hues = { 0, 1, 2, 3, 4, 5, 6, 7 },
	lootGroups = {
		{
			groups = {
				{group = "fire_spider_boss", chance = 10000000}
			},
			lootChance = 10000000
		}
	},

	primaryWeapon = "object/weapon/ranged/creature/creature_spit_heavy_flame.iff",
	secondaryWeapon = "unarmed",
	conversationTemplate = "",
	primaryAttacks = { {"stunattack",""}, {"intimidationattack",""}, {"creatureareakill",""} },
	secondaryAttacks = { {"strongpoison",""}, {"stunattack",""} }
}

CreatureTemplates:addCreatureTemplate(fire_spider_mini_boss, "fire_spider_mini_boss")
