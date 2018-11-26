/*
 * I don't actually use this in the project, but
 * I'm keeping the code for future reference.
 *
 */
#ifndef SCRAMBLE_LOCATOR_HH
#define SCRAMBLE_LOCATOR_HH

#include <memory>
namespace scramble {

        template<typename T>
        struct locator {

                static void provide(T *newService)
                {
                        service.reset(newService);
                }

                static T *current()
                {
                        return service.get();
                }

        private:
                static std::unique_ptr<T> service;
        };

        /*
         * Static templated data member has to be defined out of line
         */
        template<typename T>
        std::unique_ptr<T> locator<T>::service = nullptr;
}

#endif // SCRAMBLE_LOCATOR_H
