#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T> 
class Singleton
{
public:
    static T* instance()	{
        static T m_instance;
        return &m_instance;
    }

protected:
    Singleton(){}
    virtual ~Singleton(){}

private:

    Singleton(const Singleton<T> &);
    Singleton& operator = (const Singleton<T> &);
};

#endif//SINGLETON_H
