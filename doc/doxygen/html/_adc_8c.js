var _adc_8c =
[
    [ "AdcCalibrationInfo_t", "_adc_8c.html#struct_adc_calibration_info__t", [
      [ "gainFactor", "_adc_8c.html#a999d037cccba5fe22509aebbcdead954", null ],
      [ "offset", "_adc_8c.html#a59f874d10bf763992bd4b427fdb7f0cd", null ],
      [ "temp30Ref15V", "_adc_8c.html#ae1881bd30fdccd7a3eaf7c89cd179929", null ],
      [ "temp30Ref20V", "_adc_8c.html#a525ebcdb248ab202cf8e8fcf825ad8f1", null ],
      [ "temp30Ref25V", "_adc_8c.html#a8b2f4d8b29608186d4d2196fdeb6c181", null ],
      [ "temp85Ref15V", "_adc_8c.html#a99aaad1b4c280ab7a866ddc80353dbdf", null ],
      [ "temp85Ref20V", "_adc_8c.html#a8cdda50992a0eab696b7bb14d7e0dd70", null ],
      [ "temp85Ref25V", "_adc_8c.html#a334a88fc1db5f378d738b256a9f81695", null ]
    ] ],
    [ "AdcReferenceCalibrationInfo_t", "_adc_8c.html#struct_adc_reference_calibration_info__t", [
      [ "ref15VFactor", "_adc_8c.html#a08dd7d7df0be6068434590f3d9a741a7", null ],
      [ "ref20VFactor", "_adc_8c.html#a8c3f7fa4a0aab66c3fcbbf5a388e90b8", null ],
      [ "ref25VFactor", "_adc_8c.html#a4e3f841703120ea71fe694ed7553c74e", null ]
    ] ],
    [ "ADC_CAL_TLV_LENGTH", "_adc_8c.html#abad1a0eb2456fa57969581c4ea05bed8", null ],
    [ "ADC_MAX_RESULT", "_adc_8c.html#a3fe891ff1171ff2d3d53393ad06cfb68", null ],
    [ "ADC_REF_CAL_TLV_LENGTH", "_adc_8c.html#abb01a1630fecb54d3ec480a859412cb9", null ],
    [ "ADC_TEMP_SCALE_FACTOR", "_adc_8c.html#a6da9d6f60495f087b317dcf0c0acd085", null ],
    [ "AdcCalInfoResult_t", "_adc_8c.html#a134691b10b538ea12ffb694ca7109607", [
      [ "AdcCalInfoResult_FAILURE", "_adc_8c.html#a134691b10b538ea12ffb694ca7109607a38df2450849e38a38f5941b587516ed1", null ],
      [ "AdcCalInfoResult_SUCCESS", "_adc_8c.html#a134691b10b538ea12ffb694ca7109607a5997c4f67a470659c271667ad040c1d0", null ]
    ] ],
    [ "_AdcIsr", "_adc_8c.html#a349938431b6c84cf2227ad3d102c5e08", null ],
    [ "_GetAdcCalibrationInfo", "_adc_8c.html#a5501d5342b5fe5f3e9742e7340cf2b1a", null ],
    [ "_GetReferenceCalibrationInfo", "_adc_8c.html#a85bdfc5285f049b4d00c840fef33af58", null ],
    [ "_MoveSamplesToUserLocation", "_adc_8c.html#ad41a7943b5c26fbe607328388a72bfa3", null ],
    [ "Adc_ApplyCorrection", "_adc_8c.html#a0a024b2fd783988c7108d17c5532b09b", null ],
    [ "Adc_Config", "_adc_8c.html#aeb038bcf326000ca3a320d29351100a3", null ],
    [ "Adc_ConvertValueToTemp", "_adc_8c.html#a44545a68fa78a91a8e872863c0670522", null ],
    [ "Adc_GetGainFactor", "_adc_8c.html#a0842a80966234b58516235a957b58083", null ],
    [ "Adc_GetNumberOfConversions", "_adc_8c.html#ae5b5233dec820cd89c8ee46ad8266cde", null ],
    [ "Adc_GetOffset", "_adc_8c.html#ada428cf10ffb15cbb03e1ea5daa36786", null ],
    [ "Adc_GetTempCalib", "_adc_8c.html#ab5bb9ded6c4d0124cb23c44d21109009", null ],
    [ "Adc_Init", "_adc_8c.html#ad5f7fb41187df370fba496edbf10778d", null ],
    [ "Adc_IsConversionInProgress", "_adc_8c.html#a6f94bbae1b16f5347055058a72896411", null ],
    [ "Adc_SetChannelSequence", "_adc_8c.html#a8201a8154546752a07f766fa604c77d1", null ],
    [ "Adc_SingleConversionSequenceBlocking", "_adc_8c.html#afe5cd10dd67ef2ad4ade4f71a6ada010", null ],
    [ "Adc_StartConversionSequenceOfChannels", "_adc_8c.html#adc40c1d1983f3470f32d9d2b6f999285", null ],
    [ "Adc_StopConversion", "_adc_8c.html#a48bd94f6c7b5c13bae12ee2ae824145b", null ],
    [ "_adcCalibrationInfo", "_adc_8c.html#ad49c36821ae9dcacd8ec839385dd8d4b", null ],
    [ "_conversionInProgress", "_adc_8c.html#ad9452ac0b5fe2afc071675347e6fd157", null ],
    [ "_initialized", "_adc_8c.html#a92eaaf19cc914d995fb9c256861d5af6", null ],
    [ "_numberOfConversions", "_adc_8c.html#a2433521b8b10790808b129d28e6a3d10", null ],
    [ "_numChannelsConfigured", "_adc_8c.html#aff8af90bb6359ac2ac009af4321f167d", null ],
    [ "_refCalibrationInfo", "_adc_8c.html#a5b6fc0e53414202198ccfe97b2815dd8", null ],
    [ "_resultDestinations", "_adc_8c.html#ab4a4fd4c90ae77158d896d2cd527f299", null ],
    [ "_selectedReferenceLevel", "_adc_8c.html#ac89c23e26efc32cdc3607f0b21f601e6", null ],
    [ "_temperatureGainFactorRef15V", "_adc_8c.html#a8ca3bd83accb79971bf4ba9a2f4007d7", null ],
    [ "_temperatureGainFactorRef20V", "_adc_8c.html#ab171a027f58e4a7295193b5eb7cee1e6", null ],
    [ "_temperatureGainFactorRef25V", "_adc_8c.html#ad6cadfa7ac3fef55ec76480717702619", null ],
    [ "_useInternalReference", "_adc_8c.html#acfe84b0b860b2313bce3a0361eb24adc", null ],
    [ "_useTempSensor", "_adc_8c.html#a1e627de14f7d885d6ad06261b732882a", null ]
];