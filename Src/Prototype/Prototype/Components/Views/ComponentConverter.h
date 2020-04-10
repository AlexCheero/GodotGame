#pragma once

#include <core/Godot.hpp>
#include <Object.hpp>

#include "../SimpleComponents.h"
#include "../AttackComponents.h"

#include "../../Utils/Utils.h"

#define DECLARE_CONVERT_TO_COMPONENT(count) template<typename T, int fieldsCount> \
inline typename std::enable_if<fieldsCount == count, T>::type \
ConvertToComponent(godot::Array& arr)

DECLARE_CONVERT_TO_COMPONENT(1) { godot::Godot::print("convert component 1"); return { arr[0] }; }
DECLARE_CONVERT_TO_COMPONENT(2) { godot::Godot::print("convert component 2"); return { arr[0], arr[1] }; }
DECLARE_CONVERT_TO_COMPONENT(3) { godot::Godot::print("convert component 3"); return { arr[0], arr[1], arr[2] }; }
DECLARE_CONVERT_TO_COMPONENT(4) { godot::Godot::print("convert component 4"); return { arr[0], arr[1], arr[2], arr[3] }; }
DECLARE_CONVERT_TO_COMPONENT(5) { godot::Godot::print("convert component 5"); return { arr[0], arr[1], arr[2], arr[3], arr[4] }; }