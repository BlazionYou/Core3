object_tangible_deed_generator_deed_generator_fusion_heavy_deed = object_tangible_deed_generator_deed_shared_generator_fusion_deed:new {
	templateType = STRUCTUREDEED,
	placeStructureComponent = "PlaceStructureComponent",
	generatedObjectTemplate = "object/installation/generators/power_generator_fusion_style_1.iff",



	numberExperimentalProperties = {1, 1, 1, 2, 1},
	experimentalProperties = {"XX", "XX", "XX", "MA", "UT", "XX"},
	experimentalWeights = {1, 1, 1, 1, 2, 1},
	experimentalGroupTitles = {"null", "null", "null", "expStorage", "null"},
	experimentalSubGroupTitles = {"null", "null", "extractrate", "hoppersize", "hitpoints"},
	experimentalMin = {0, 0, 60, 500000, 1000},
	experimentalMax = {0, 0, 180, 2500000, 5000},
	experimentalPrecision = {0, 0, 0, 0, 0},
	experimentalCombineType = {0, 0, 1, 1, 4},
}

ObjectTemplates:addTemplate(object_tangible_deed_generator_deed_generator_fusion_heavy_deed, "object/tangible/deed/generator_deed/generator_fusion_heavy_deed.iff")
