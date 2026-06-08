
object_draft_schematic_weapon_kinetic_saber_kinetic_saber_gen5 = object_draft_schematic_weapon_kinetic_saber_shared_kinetic_saber_gen5:new {

   templateType = DRAFTSCHEMATIC,

   customObjectName = "Fifth Generation Kinetic Saber",

   craftingToolTab = 1,
   complexity = 20, 
   size = 1, 
   factoryCrateType = "object/factory/factory_crate_weapon.iff",
   
   xpType = "crafting_weapons_general", 
   xp = 1200, 

   assemblySkill = "weapon_assembly", 
   experimentingSkill = "weapon_experimentation", 
   customizationSkill = "weapon_customization", 
   factoryCrateSize = 0,

   customizationOptions = {},
   customizationStringNames = {},
   customizationDefaults = {},

   ingredientTemplateNames = {"craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n"},
   ingredientTitleNames = {"frame_assembly", "previous_gen_saber", "activator_coil", "handgrip", "crystal_array", "power_field_insulator", "energizers"},
   ingredientSlotType = {0, 1, 0, 0, 1, 0, 0},
   resourceTypes = {"steel_duralloy", "object/weapon/melee/sword/kinetic_saber/shared_kinetic_saber_gen4.iff", "aluminum_titanium", "petrochem_inert_polymer", "object/tangible/component/weapon/lightsaber/shared_lightsaber_module_force_crystal.iff", "gas_inert_culsion", "copper_polysteel"},
   resourceQuantities = {45, 1, 25, 30, 1, 30, 30},
   contribution = {100, 100, 100, 100, 100, 100, 100},

   targetTemplate = "object/weapon/melee/sword/kinetic_saber/shared_kinetic_saber_gen5.iff",

   additionalTemplates = {}
}
ObjectTemplates:addTemplate(object_draft_schematic_weapon_kinetic_saber_kinetic_saber_gen5, "object/draft_schematic/weapon/kinetic_saber/kinetic_saber_gen5.iff")
