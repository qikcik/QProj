#pragma once
#include <memory>
#include <dependency/uuid.h>

struct OwnerPtrBlock
{
    void* obj {nullptr};
    int count {0};

    uuids::uuid  uuid {};
    static std::unordered_map<uuids::uuid,OwnerPtrBlock*> blocks;

    OwnerPtrBlock(const uuids::uuid& inUUID)
    {
        if(inUUID.is_nil())
        {
            std::random_device rd;
            auto seed_data = std::array<int, 6>{};
            std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
            std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
            std::ranlux48_base generator(seq);

            uuids::basic_uuid_random_generator <std::ranlux48_base> gen(&generator);
            uuid = gen();
        }
        else
            uuid = inUUID;
        blocks[uuid] = this;
    }
    OwnerPtrBlock() {
        std::random_device rd;
        auto seed_data = std::array<int, 6> {};
        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        std::ranlux48_base generator(seq);

        uuids::basic_uuid_random_generator<std::ranlux48_base> gen(&generator);
        uuid = gen();

        blocks[uuid] = this;
    }

    ~OwnerPtrBlock() {
        blocks.erase(uuid);
    }

    template<class T>
    static void doDelete(OwnerPtrBlock* block)
    {
        if(!block) return;

        if(block->obj)
            delete reinterpret_cast<T*>(block->obj);

        block->count--;
        if(block->count <= 0)
            delete block;
    }

    static void weakDoDelete(OwnerPtrBlock* block)
    {
        if(!block) return;

        block->count--;
        if(block->count <= 0)
            delete block;
    }

    static OwnerPtrBlock* getBlockByUUID(uuids::uuid inUUID)
    {
        if( auto it = blocks.find(inUUID) ; it != blocks.end() )
        {
            return it->second;
        }
        return nullptr;
    }
};

template<typename T>
class OwnerPtr
{
public:
    using element_type = T;

    OwnerPtr()
    {
        block = new OwnerPtrBlock();
        block->count++;
    }
    OwnerPtr(const uuids::uuid& inUUID)
    {
        block = new OwnerPtrBlock(inUUID);
        block->count++;
    }
    OwnerPtr(T* obj,const uuids::uuid& inUUID = uuids::uuid{})
    {
        block = new OwnerPtrBlock(inUUID);
        block->obj = obj;
        block->count++;
    }
    ~OwnerPtr()
    {
        OwnerPtrBlock::doDelete<T>(block);
    }

    template<typename TOther>
    OwnerPtr(const OwnerPtr<TOther>& other)  = delete;

    template<typename TOther>
    OwnerPtr& operator=(const OwnerPtr<TOther>& other) = delete;

    template<typename TOther>
    OwnerPtr(OwnerPtr<TOther>&& other)
    {
        block = other.block;
        other.block = nullptr;
    }

    template<typename TOther>
    OwnerPtr<T>& operator=(OwnerPtr<TOther>&& other) noexcept
    {
        //if(this == &other)
        //    return *this;

        this->~OwnerPtr<T>();
        new (this) OwnerPtr<T>(std::move(other));
        return *this;
    }

    T* get()
    {
        if(!block) {
            return nullptr;
        };

        return static_cast<T*>(block->obj);
    }

    uuids::uuid getBlockUUID() const
    {
        if(!block) return uuids::uuid{};
        return block->uuid;
    }


    void set(T* obj)
    {
        if(!block) return;
        block->obj = obj;
    }

    OwnerPtrBlock* block {nullptr};
};

template<typename T>
class WeakPtr
{
public:
    using weakElement_type = T;

    WeakPtr() {};
    ~WeakPtr() {
        OwnerPtrBlock::weakDoDelete(block);
    };

    WeakPtr(const WeakPtr<T>& other)
    {
        block = other.block;

        if(block)
            block->count++;
        blockUUID = other.blockUUID;
    };

    WeakPtr(WeakPtr<T>&& other)
    {
        if(this == &other)
            return;

        block = other.block;
        blockUUID = other.blockUUID;

        other.block = nullptr;
        other.blockUUID = uuids::uuid{};
    };

    WeakPtr& operator=(const WeakPtr<T>& other)
    {
        if(this == &other)
            return *this;

        this->~WeakPtr<T>();
        new (this) WeakPtr<T>(other);
        return *this;
    }

    WeakPtr& operator=(WeakPtr<T>&& other) noexcept
    {
        if(this == &other)
            return *this;

        this->~WeakPtr<T>();
        new (this) WeakPtr<T>(std::move(other));
        return *this;
    }

    template<typename Tother>
    explicit WeakPtr(const OwnerPtr<Tother>& owner)
    {
        block = owner.block;
        blockUUID = owner.getBlockUUID();
        block->count++;
    };

    uuids::uuid getBlockUUID() const
    {
        return blockUUID;
    }

    template<typename Tother>
    WeakPtr& operator=(const OwnerPtr<Tother>& other)
    {
        this->~WeakPtr<T>();
        block = other.block;
        blockUUID = other.getBlockUUID();
        block->count++;
        return *this;
    }

    T* get()
    {
        if(!block) {
            if(!blockUUID.is_nil())
            {
                block = OwnerPtrBlock::getBlockByUUID(blockUUID);
                if(!block) return nullptr;
            }
            return nullptr;
        };

        return static_cast<T*>(block->obj);
    }

    uuids::uuid  blockUUID {};
    OwnerPtrBlock* block {nullptr};
};