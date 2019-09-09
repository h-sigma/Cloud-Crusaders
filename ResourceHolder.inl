
template <typename Resource, typename Identifier> void ResourceHolder<Resource,Identifier>::load(Identifier id , const std::string& path)
{
    std::unique_ptr<Resource> tempResource = std::make_unique<Resource>();
    if( !tempResource->loadFromFile(path) )
    {
        throw std::runtime_error("ResourceHolder::load - failed to load. Path: " + path);
    }
    if(mResourceMap.find(id) != mResourceMap.end())
    return;
    auto inserted = mResourceMap.insert( std::make_pair ( id , std::move(tempResource) ) ) ;
    assert( inserted.second );
}

template <typename Resource, typename Identifier> template <typename Parameter> void ResourceHolder<Resource,Identifier>::load(Identifier id , const std::string& path , const Parameter& param)
{
    std::unique_ptr<Resource> tempResource = std::make_unique<Resource>();
    if( !tempResource->loadFromFile(path , param))
    {
        throw std::runtime_error("ResourceHolder::load - failed to load. Path: " + path);
    }
    if(mResourceMap.find(id) != mResourceMap.end())
    return;
    auto inserted = mResourceMap.insert( std::make_pair( id , std::move( tempResource ) ) );
    assert(inserted.second);
}

template <typename Resource, typename Identifier>  Resource& ResourceHolder<Resource,Identifier>::get(Identifier id)
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    return *found->second;
}

template <typename Resource, typename Identifier> const Resource& ResourceHolder<Resource,Identifier
>::get(Identifier id) const
{
    auto found = mResourceMap.find(id);
    return *found->second;
}
