#ifndef RESOURCEHOLDER_HPP
#define RESOURCEHOLDER_HPP


#include <map>
#include <stdexcept>
#include <cassert>
#include <memory>
#include <string>


template <typename Resource, typename Identifier> class ResourceHolder{
    public:
        void load(Identifier id , const std::string& path);
        template <typename Parameter> void load(Identifier id, const std::string& path, const Parameter& param);
        Resource& get(Identifier id);
        const Resource& get(Identifier id) const;
    private:
        std::map< Identifier , std::unique_ptr<Resource> > mResourceMap;
};

#include "ResourceHolder.inl"
#endif