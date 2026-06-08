
object_draft_schematic_weapon_kinetic_saber_kinetic_saber_training = object_draft_schematic_weapon_kinetic_saber_shared_kinetic_saber_training:new {

   templateType = DRAFTSCHEMATIC,

   customObjectName = "A Training Kinetic Saber",

   craftingToolTab = 1, -- weaponsmith tab
   complexity = 16, 
   size = 1, 
   factoryCrateType = "object/factory/factory_crate_weapon.iff",
   
   xpType = "crafting_weapons_general", 
   xp = 200, 

   assemblySkill = "weapon_assembly", 
   experimentingSkill = "weapon_experimentation", 
   customizationSkill = "weapon_customization", 
   factoryCrateSize = 0,   

   customizationOptions = {},
   customizationStringNames = {},
   customizationDefaults = {},

   ingredientTemplateNames = {"craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n"},
   ingredientTitleNames = {"frame_assembly", "crystal_array", "activator_coil", "handgrip", "focusing_crystals", "power_field_insulator", "energizers"},
   -- SLOTS: 0=resource, 1=component (IdenticalSlot → ComponentSlot), 3=generic component
   ingredientSlotType = {0, 1, 0, 0, 1, 0, 0},
   -- Slot 1: force crystal, Slot 4: krayt pearl (or color crystal)
   resourceTypes = {"mineral", "object/tangible/component/weapon/lightsaber/shared_lightsaber_module_force_crystal.iff", "metal", "chemical", "object/tangible/component/weapon/lightsaber/shared_lightsaber_module_krayt_dragon_pearl.iff", "gas", "metal"},
   resourceQuantities = {10, 1, 10, 12, 1, 10, 10},
   contribution = {100, 100, 100, 100, 100, 100, 100},


   targetTemplate = "object/weapon/melee/sword/kinetic_saber/shared_kinetic_saber_training.iff",

   additionalTemplates = {}

}
ObjectTemplates:addTemplate(object_draft_schematic_weapon_kinetic_saber_kinetic_saber_training, "object/draft_schematic/weapon/kinetic_saber/kinetic_saber_training.iff")
