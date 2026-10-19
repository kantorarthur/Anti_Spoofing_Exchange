#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <cstdint>
#include <memory>
#include <limits>
#include "object_pool.h";

enum class TimeInForce{
    GTC, IOC, FOK, AON
};


using OrderId = std::uint64_t;
using AccountId = std::uint64_t;
using SymbolId = std::uint64_t;
using Price = std::int64_t;
using Quantity = std::int64_t;
using Flags = std::uint8_t;

using Volume = std::int64_t;

using BitMap = std::uint64_t;

using BestBid = std::int64_t;
using BestAsk = std::int64_t;

using BidLevel = std::uint32_t;
using AskLevel = std::uint32_t;

struct alignas(64) Order{

    OrderId order_id; //8b
    AccountId account_id; //8b
    SymbolId symbol_id; //8b
    Price price; //8b
    Quantity quantity; //8b
    TimeInForce TIF = TimeInForce::GTC; //4b
    Flags flags; //1b
    uint8_t padding[19]; //19b

    bool is_buy() const { return flags & 0x01; }
    bool is_limit() const { return flags & 0x02; }

    void set_buy(bool order_type) { if (order_type) flags |= 0x01;
                                    else flags &= ~0x01;}
    void set_limit(bool order_type) { if (order_type) flags |= 0x02;
                                      else flags &= ~0x02; }
    //I consider bit:
    // bit1 - If bit1 it's 1, it's bid, else it's ask
    // bi2 - If bit2 it's 1, it's limit order, else it's cross

    //In set_buy I set bit1 to 1 if it's ask, else to 0
    //In set_limit I set bit2 to 1 if it's limit, else to 0

}; 
static_assert(sizeof(Order) == 64 && "Size of order struct it's not 64 bytes");

struct PriceLevelFIFO{
    uint32_t head = NULL_INDEX;
    uint32_t tail = NULL_INDEX;
    uint32_t count = 0;
    Volume total_volume = 0;

    inline bool is_empty() const { return head == NULL_INDEX; }
};

class OrderBook{
private:

    std::unique_ptr<PriceLevelFIFO[]> bid_levels;
    std::unique_ptr<PriceLevelFIFO[]> ask_levels;

    std::unique_ptr<BitMap> bid_bitmap;
    std::unique_ptr<BitMap> ask_bitmap;

    BestBid best_bid = -1;
    BestAsk best_ask = std::numeric_limits<BestAsk>::max();

    BidLevel bid_level_count = 0;
    AskLevel ask_level_count = 0;

};


#endif
