#include "mm_feature.h"

MMFeature::MMFeature() {
}

MMFeature::~MMFeature() {
}

void MMFeature::normalize(float* p_data) const {
    if (!p_data) {
        ERR_PRINT_ONCE("Invalid data provided in normalize.");
        return;
    }
    switch (normalization_mode) {
    case Standard:
        _normalize_standard(p_data);
        break;
    case MinMax:
        _normalize_minmax(p_data);
        break;
    case Raw:
    default:
        break;
    }
}

void MMFeature::denormalize(float* p_data) const {
    if (!p_data) {
        ERR_PRINT_ONCE("Invalid data provided in denormalize.");
        return;
    }
    switch (normalization_mode) {
    case Standard:
        _denormalize_standard(p_data);
        break;
    case MinMax:
        _denormalize_minmax(p_data);
        break;
    case Raw:
    default:
        break;
    }
}

void MMFeature::_normalize_minmax(float* p_data) const {
    if (!p_data) {
        ERR_PRINT_ONCE("Invalid data provided in _normalize_minmax.");
        return;
    }
    for (int64_t i = 0; i < get_dimension_count(); ++i) {
        const float delta = maxes[i] - mins[i];
        if (abs(delta) < KINDA_SMALL_NUMBER) {
            continue;
        }
        p_data[i] = (p_data[i] - mins[i]) / delta;
    }
}

void MMFeature::_denormalize_minmax(float* p_data) const {
    if (!p_data) {
        ERR_PRINT_ONCE("Invalid data provided in _denormalize_minmax.");
        return;
    }
    for (int64_t i = 0; i < get_dimension_count(); ++i) {
        const float delta = maxes[i] - mins[i];
        if (abs(delta) < KINDA_SMALL_NUMBER) {
            continue;
        }
        p_data[i] = (p_data[i] * delta) + mins[i];
    }
}

void MMFeature::_normalize_standard(float* p_data) const {
    if (!p_data) {
        ERR_PRINT_ONCE("Invalid data provided in _normalize_standard.");
        return;
    }
    for (int64_t i = 0; i < get_dimension_count(); ++i) {
        p_data[i] = (p_data[i] - means[i]) / (std_devs[i] + KINDA_SMALL_NUMBER);
    }
}

void MMFeature::_denormalize_standard(float* p_data) const {
    if (!p_data) {
        ERR_PRINT_ONCE("Invalid data provided in _denormalize_standard.");
        return;
    }
    ERR_FAIL_COND(std_devs.size() != get_dimension_count());
    for (int64_t i = 0; i < get_dimension_count(); ++i) {
        p_data[i] = (p_data[i] * (std_devs[i] + KINDA_SMALL_NUMBER)) + means[i];
    }
}

void MMFeature::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_dimension_count"), &MMFeature::get_dimension_count);
    ClassDB::bind_method(D_METHOD("set_normalization_mode", "value"), &MMFeature::set_normalization_mode, DEFVAL(MMFeature::NormalizationMode::Standard));
    ClassDB::bind_method(D_METHOD("get_normalization_mode"), &MMFeature::get_normalization_mode);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "normalization_mode", PROPERTY_HINT_ENUM, "Raw,Standard,MinMax", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_ALWAYS_DUPLICATE), "set_normalization_mode", "get_normalization_mode");

    BINDER_PROPERTY_PARAMS(MMFeature, Variant::FLOAT, weight);
    BINDER_PROPERTY_PARAMS(MMFeature, Variant::PACKED_FLOAT32_ARRAY, means, PROPERTY_HINT_NONE, "", DEBUG_PROPERTY_STORAGE_FLAG);
    BINDER_PROPERTY_PARAMS(MMFeature, Variant::PACKED_FLOAT32_ARRAY, std_devs, PROPERTY_HINT_NONE, "", DEBUG_PROPERTY_STORAGE_FLAG);
    BINDER_PROPERTY_PARAMS(MMFeature, Variant::PACKED_FLOAT32_ARRAY, maxes, PROPERTY_HINT_NONE, "", DEBUG_PROPERTY_STORAGE_FLAG);
    BINDER_PROPERTY_PARAMS(MMFeature, Variant::PACKED_FLOAT32_ARRAY, mins, PROPERTY_HINT_NONE, "", DEBUG_PROPERTY_STORAGE_FLAG);

    BIND_ENUM_CONSTANT(Raw);
    BIND_ENUM_CONSTANT(Standard);
    BIND_ENUM_CONSTANT(MinMax);
}

VARIANT_ENUM_CAST(MMFeature::NormalizationMode);
