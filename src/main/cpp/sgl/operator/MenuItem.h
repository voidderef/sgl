#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <utility>

namespace sgl {

class MenuPage;

class MenuItem
{
public:
    class DefaultCallbacks
    {
    public:
        template<ssize_t v>
        static ssize_t MenuPageChild() {
            return v;
        }

        static ssize_t MenuPageBack() {
            return -1;
        }

        static ssize_t Dummy() {
            return 0;
        }

    private:
        DefaultCallbacks() = default;
        ~DefaultCallbacks() = default;
    };

public:
    explicit MenuItem(std::string name, const std::function<ssize_t()>& callback) :
        m_name(std::move(name)),
        m_callback(callback)
    {};

    virtual ~MenuItem() = default;

    const std::string& GetName() const {
        return m_name;
    }

    virtual void Activate(bool activate) = 0;

    virtual void SetHighlighted(bool highlighted) = 0;

    virtual ssize_t Selected() {
        if (m_callback) {
            return m_callback();
        } else {
            return 0;
        }
    };

private:
    const std::string m_name;
    const std::function<ssize_t()> m_callback;
};

}
