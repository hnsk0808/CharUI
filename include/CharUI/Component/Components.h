#pragma once
#include <memory>
#include "Canvas.h"
#include "Image.h"
#include "Grid.h"
#include "ProgressBar.h" 
#include "HBox.h" 
#include "Text.h"
#include "VBox.h"

namespace cui
{

template<class...T>
auto canvas(T&&...t) { return std::make_shared<Canvas>(std::forward<T>(t)...); }

template<class...T>
auto image(T&&...t) { return std::make_shared<Image>(std::forward<T>(t)...); }

template<class...T>
auto progressBar(T&&...t) { return std::make_shared<ProgressBar>(std::forward<T>(t)...); }

template<class...T>
auto text(T&&...t) { return std::make_shared<Text>(std::forward<T>(t)...); }

// Layout-Component

template<class...T>
auto grid(T&&...t) { return std::make_shared<Grid>(std::forward<T>(t)...); }

template<class...T>
auto hBox(T&&...t) { return std::make_shared<HBox>(HBox{ std::forward<T>(t)... }); }

template<class...T>
auto vBox(T&&...t) { return std::make_shared<VBox>(VBox{ std::forward<T>(t)... }); }

}