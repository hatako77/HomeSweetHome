template<typename T>
class IRepository
{
public:
    virtual bool add(const T&) = 0;
    virtual bool update(const T&) = 0;
    virtual bool remove(uint16_t id) = 0;
    virtual T* get(uint16_t id) = 0;

    virtual ~IRepository() = default;
};
