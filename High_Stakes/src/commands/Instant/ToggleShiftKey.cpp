#include "ToggleShiftKey.hpp"
#include "../../Config.hpp"

ToggleShiftKey::ToggleShiftKey() {

}

void ToggleShiftKey::execute() {
    Constants::shifted = !Constants::shifted;
}
