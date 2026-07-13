class RoomManager
{
public:
    void begin();

    uint16_t count() const;

    Room* get(uint16_t id);
    const Room* get(uint16_t id) const;

    Room* getAt(uint16_t index);
    const Room* getAt(uint16_t index) const;

    Room* add(const Room& room);
    bool update(const Room& room);
    bool remove(uint16_t id);

    bool save();
    bool load();

private:
    static constexpr uint8_t MAX_ROOMS=32;

    Room rooms[MAX_ROOMS];

    uint16_t roomCount=0;
    uint16_t nextId=1;
};

extern RoomManager roomManager;
