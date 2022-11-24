#ifndef IOC_H
#define IOC_H


#include <functional>
#include <map>
#include <memory>


using namespace std;

class IOCContainer

{
    static int s_typeId; // определяем статическую переменную для хранения идентификатора типа следующего вызываемого объекта

public:

    template<typename T> static int GetTypeID() {
            // получение экземпляра объекта
            static int typeId = s_typeId++;
            return typeId;
        }

    class FactoryBase{
        // абстрактный базовый класс, из которого будут производиться конкретные фабрики
        // будем хранить все фабрики в одной коллекции
    public:
        virtual ~FactoryBase() =default;
    };

    map<int, shared_ptr<FactoryBase>> factories; // хранение фабрик в контейнере map

    template<typename T> class CFactory : public FactoryBase
        {
            std::function<std::shared_ptr<T>()> functor;
        public:
            ~CFactory() {}

            CFactory(std::function<std::shared_ptr<T>()> functor) :functor(functor)
            {

            }

            std::shared_ptr<T> GetObject()
            {
                return functor();
            }

        };

    template<typename T> std::shared_ptr<T> GetObject()
    {
        auto typeId = GetTypeID<T>();
        auto factoryBase = factories[typeId];
        auto factory = std::static_pointer_cast<CFactory<T>>(factoryBase);
        return factory->GetObject();
    }


    // регистрация экземпляров, используя функтор

    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)> functor)
    {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return functor(GetObject<TS>()...); });
    }

    template<typename TInterface> //Регистрация экземпляра
    void RegisterInstance(std::shared_ptr<TInterface> t)
    {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return t; });
    }

    template<typename TInterface, typename ...TS> // Будем работать с указателем на функцию
    void RegisterFunctor(std::shared_ptr<TInterface>(*functor)(std::shared_ptr<TS> ...ts))
    {
        RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)>(functor));
    }

    static IOCContainer& IOCInstance() //static - для получения единственного экземпляра объекта
        {
            static IOCContainer ioc_;
            return ioc_;
        }

    //Фабрика для вызова необходимого конструктора для каждого экземпляра
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterFactory()
    {
        RegisterFunctor(
            std::function<std::shared_ptr<TInterface>(std::shared_ptr<TArguments> ...ts)>(
                [](std::shared_ptr<TArguments>...arguments) -> std::shared_ptr<TInterface>
        {
            return std::make_shared<TConcrete>(std::forward<std::shared_ptr<TArguments>>(arguments)...);
        }));

    }





    //Фабрика, возвращающая единственный экземпляр объекта
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterInstance()
    {
        RegisterInstance<TInterface>(std::make_shared<TConcrete>(GetObject<TArguments>()...));
    }



};

IOCContainer gContainer;

int IOCContainer::s_typeId = 1; // инициализируем ненулевым значением

#endif // IOC_H
