#include "Menu.h"

namespace sgl {

Menu::Menu(std::string name, MenuPage* rootPage) :
    m_name(std::move(name)),
    m_root(rootPage),
    m_refCurrentPage(m_root),
    m_active(true)
{
    m_root->Activate(true);
}

Menu::~Menu()
{
    delete m_root;
}

void Menu::Activate(bool activate)
{
    m_active = activate;
    m_root->Activate(m_active);
}

void Menu::SelectNextItem()
{
    if (m_active) {
        m_refCurrentPage->NextItem();
    }
}

void Menu::SelectPreviousItem()
{
    if (m_active) {
        m_refCurrentPage->PreviousItem();
    }
}

bool Menu::ConfirmCurrentItem()
{
    if (m_active) {
        MenuItem* item = m_refCurrentPage->GetCurrentItem();

        ssize_t subPageIdx = item->Selected();

        // item references parent or child sub page
        if (subPageIdx != 0) {
            m_refCurrentPage->Activate(false);

            if (subPageIdx == -1) {
                m_refCurrentPage = m_refCurrentPage->GetRefParent();

                // go back from root to non existing parent -> exit menu
                if (!m_refCurrentPage) {
                    return false;
                }
            } else {
                // index starts at 1, not 0

                m_refCurrentPage = m_refCurrentPage->GetRefChildPage(static_cast<size_t>(subPageIdx - 1));
            }

            m_refCurrentPage->Activate(true);
        }
    }

    return true;
}

}