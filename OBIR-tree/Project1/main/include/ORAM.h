#ifndef ORAM_H
#define ORAM_H

#include <string>
#include <unordered_map>

class ORAM {
public:
    ORAM() {}
    virtual ~ORAM() {}

    virtual std::string get(const uint32_t & key) = 0;
    virtual std::string getAndRemove(const uint32_t & key, const uint32_t& pos) = 0;
    virtual void put(const uint32_t& key, const std::string & value) = 0;
    virtual void put(const uint32_t& key, const std::string & value, const uint32_t& pos) = 0;

	virtual void bulkLoad(std::unordered_map<uint32_t, std::string>& blocks) = 0;
	virtual void batchAccess(std::unordered_map<uint32_t, std::string>& blocks) = 0;

    virtual size_t getServerSize() const = 0;
    virtual size_t getClientSize() const = 0;
    virtual size_t getCommSize() const = 0;
    virtual void resetCommSize() = 0;
    virtual double getCacheHitRate() const = 0;
    virtual double getAccidentHitRate() const = 0;
    virtual void resetCacheHitRate() = 0;
    virtual size_t getHitCount() const = 0;
    virtual size_t getAccessCount() const = 0;
    virtual size_t getAccidentHitCount() const = 0;
};

#endif //ORAM_H
