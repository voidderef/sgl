#pragma once

#include "MenuItem.h"
#include "MenuPage.h"

namespace sgl {

class Menu
{
public:
    Menu(std::string name, MenuPage* rootPage);

    ~Menu();

    void Activate(bool activate);

    void SelectNextItem();

    void SelectPreviousItem();

    bool ConfirmCurrentItem();

private:
    const std::string m_name;
    MenuPage* m_root;

    MenuPage* m_refCurrentPage;
    bool m_active;
};

}
