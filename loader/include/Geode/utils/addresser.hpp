#pragma once

/**
 * Adapted from https://gist.github.com/altalk23/29b97969e9f0624f783b673f6c1cd279
 */

#include "../utils/casts.hpp"
#include "casts.hpp"

#include <Geode/DefaultInclude.hpp>
#include <cocos2d.h>
#include <cstdlib>
#include <stddef.h>
#include <type_traits>

namespace geode::addresser {

    template <typename T>
    inline intptr_t getVirtual(T func);

    template <typename T>
    inline intptr_t getNonVirtual(T func);

    template <typename T, typename F>
    inline F thunkAdjust(T func, F self);

    template <typename T, typename F>
    inline F rthunkAdjust(T func, F self);

    template <class Class>
    Class* friendCreate() {
        return Class::create();
    }

    template <class Class>
    concept HasCreate = requires() { addresserFriendCreate<Class>(); };

    class GEODE_DLL Addresser final {
        template <char C>
        struct SingleInheritance {
            virtual ~SingleInheritance() {}
        };

        struct MultipleInheritance : SingleInheritance<'L'>, SingleInheritance<'F'> {
            virtual ~MultipleInheritance() {}
        };

        static MultipleInheritance* instance();

        template <typename R, typename T, typename... Ps>
        static ptrdiff_t indexOf(R (T::*func)(Ps...)) {
            using method_t = ptrdiff_t (T::*)();
            return (reinterpret_cast<T*>(instance())->*reinterpret_cast<method_t>(func))();
        }

        template <typename R, typename T, typename... Ps>
        static ptrdiff_t indexOf(R (T::*func)(Ps...) const) {
            return indexOf(reinterpret_cast<R (T::*)(Ps...)>(func));
        }

        template <typename T>
        static ptrdiff_t thunkOf(T ptr) {
            // msvc
            if (sizeof(T) == sizeof(ptrdiff_t)) return 0;

            // all
            auto thunk = *(reinterpret_cast<ptrdiff_t*>(&ptr) + 1);

            // arm
            if (thunk & 1) thunk >>= 1;
            return thunk;
        }

        // I gave up
        template <class Class>
            requires(HasCreate<Class>)
        static Class* generateInstance() {
            return friendCreate<Class>();
        }

        template <class Class>
            requires(!HasCreate<Class>)
        static Class* generateInstance() {
            // Create a random memory block with the size of Class
            // Assign a pointer to that block and cast it to type Class*
            uint8_t dum[sizeof(Class)]{};
            auto ptr = reinterpret_cast<Class*>(dum);
            // Now you have a object of Class that actually isn't an object of Class and is just a
            // random memory But C++ doesn't know that of course So now you can copy an object
            // that wasn't there in the first place
            // ((oh also get the offsets of the virtual tables))
            auto ins = new Class(*ptr);
            // this is how the first human was made
            return ins;
        }

        template <class Class>
            requires(HasCreate<Class>)
        static void releaseInstance(Class* ins) {}

        template <class Class>
            requires(!HasCreate<Class>)
        static void releaseInstance(Class* ins) {
            // And we delete the new instance because we are good girls
            // and we don't leak memories
            operator delete(ins);
        }

        /**
         * Specialized functionss
         */
        template <typename R, typename T, typename... Ps>
        static intptr_t addressOfVirtual(
            R (T::*func)(Ps...), typename std::enable_if_t<!std::is_abstract_v<T>>* = 0
        ) {
            using geode::cast::reference_cast;

            auto index = indexOf(func);
            auto thunk = thunkOf(func);
            auto ins = generateInstance<T>();

            // log::debug("[[" + utils::intToHex((void*)ins) + " + " + utils::intToHex(thunk) + "] +
            // " + utils::intToHex(index) + "]");
            // log::debug(
            //     "[[{} + {}] + {}]", utils::intToHex((void*)ins), utils::intToHex(thunk),
            //     utils::intToHex(index)
            // );

            // [[this + thunk] + offset] is the f we want
            auto address = *(intptr_t*)(*(intptr_t*)(reference_cast<intptr_t>(ins) + thunk) + index);

#ifdef GEODE_IS_WINDOWS
            // check if first instruction is a jmp, i.e. if the func is a thunk
            if (*reinterpret_cast<uint16_t*>(address) == 0x25ff) {
                // read where the jmp points to and jump there
                address = *reinterpret_cast<uint32_t*>(address + 2);
                // that then contains the actual address of the func
                address = *reinterpret_cast<uintptr_t*>(address);
            }
#endif
            releaseInstance<T>(ins);

            return address;
        }

        template <typename R, typename T, typename... Ps>
        static intptr_t addressOfVirtual(
            R (T::*func)(Ps...) const,
            typename std::enable_if_t<std::is_copy_constructible_v<T> && !std::is_abstract_v<T>>* = 0
        ) {
            return addressOfVirtual(reinterpret_cast<R (T::*)(Ps...)>(func));
        }

        template <typename R, typename T, typename... Ps>
        static intptr_t addressOfVirtual(
            R (T::*func)(Ps...), typename std::enable_if_t<std::is_abstract_v<T>>* = 0
        ) {
            return 0;
        }

        template <typename R, typename T, typename... Ps>
        static intptr_t addressOfVirtual(
            R (T::*func)(Ps...) const, typename std::enable_if_t<std::is_abstract_v<T>>* = 0
        ) {
            return 0;
        }

        template <typename R, typename T, typename... Ps>
        static intptr_t addressOfNonVirtual(R (T::*func)(Ps...) const) {
            return addressOfNonVirtual(reinterpret_cast<R (T::*)(Ps...)>(func));
        }

        template <typename R, typename T, typename... Ps>
        static intptr_t addressOfNonVirtual(R (T::*func)(Ps...)) {
            return geode::cast::reference_cast<intptr_t>(func);
        }

        template <typename R, typename... Ps>
        static intptr_t addressOfNonVirtual(R (*func)(Ps...)) {
            return geode::cast::reference_cast<intptr_t>(func);
        }

        template <typename T>
        friend intptr_t getVirtual(T func);

        template <typename T>
        friend intptr_t getNonVirtual(T func);

        template <typename T, typename F>
        friend F thunkAdjust(T func, F self);

        template <typename T, typename F>
        friend F rthunkAdjust(T func, F self);
    };

#ifdef GEODE_ADDRESSER_TEST
    template <typename T>
    inline intptr_t getVirtual(T func) {
        return Addresser::addressOfVirtual(func);
    }

    template <typename T>
    inline intptr_t getNonVirtual(T func) {
        return Addresser::addressOfNonVirtual(func);
    }
#else
    template <typename T>
    inline intptr_t getVirtual(T func) {
        // log::debug(
        //     "Get virtual function address from {}",
        //     utils::intToHex(geode::cast::reference_cast<intptr_t>(func))
        // );
        auto addr = Addresser::addressOfVirtual(func);
        // log::debug("The address is: {}", utils::intToHex(addr));
        return addr;
    }

    template <typename T>
    inline intptr_t getNonVirtual(T func) {
        // log::debug(
        //     "Get non-virtual function address from {}",
        //     utils::intToHex(geode::cast::reference_cast<intptr_t>(func))
        // );
        auto addr = Addresser::addressOfNonVirtual(func);
        // log::debug("The address is: {}", utils::intToHex(addr));
        return addr;
    }

    template <typename T, typename F>
    inline F thunkAdjust(T func, F self) {
        // do NOT delete the line below.
        // doing so breaks thunk adjusting on windows.
        // why? bruh idk
        auto _ = *geode::cast::template union_cast<ptrdiff_t*>(&func);
        return (F)((intptr_t)self + Addresser::thunkOf(func));
    }

    template <typename T, typename F>
    inline F rthunkAdjust(T func, F self) {
        // do NOT delete the line below.
        // doing so breaks thunk adjusting on windows.
        // why? bruh idk
        auto _ = *geode::cast::template union_cast<ptrdiff_t*>(&func);
        return (F)((intptr_t)self - Addresser::thunkOf(func));
    }
#endif
}
