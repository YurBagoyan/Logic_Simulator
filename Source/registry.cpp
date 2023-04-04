#include "Include/registry.h"

#include "Include/Elements/Gates/all.h"
#include "Include/Elements/Timers/clock.h"

#include "Include/package.h"
#include "Include/Nodes/nodePackage.h"


inline void init_resources()
{
    Q_INIT_RESOURCE(Icons);
}

struct Registry::PIMPL {
  //using Plugins = std::vector<std::shared_ptr<SharedLibrary>>;
    using MetaInfos = std::vector<MetaInfo>;
    MetaInfos metaInfos{};
//  Plugins plugins{};
//  Packages packages{};
//  fs::path app_path{};
//  fs::path system_plugins_path{};
//  fs::path user_plugins_path{};
//  fs::path system_packages_path{};
//  fs::path user_packages_path{};
};

Registry &Registry::get()
{
    static Registry s_registry{};
    return s_registry;
}

Registry::Registry()
    :   m_pimpl{ std::make_unique<PIMPL>() }
{

}

void Registry::registerInternalElements()
{
    init_resources();

    registerElement<Package, nodes::NodePackage>("Package", ":Docs/LogicIcons/package.png");

    registerElement<And>("AND (Bool)", ":Docs/GateIcons/AND.ico");
    registerElement<Nand>("NAND (Bool)", ":Docs/GateIcons/NAND.ico");
    registerElement<Nor>("NOR (Bool)", ":Docs/GateIcons/NOR.ico");
    registerElement<Not>("NOT (Bool)", ":Docs/GateIcons/NOT.ico");
    registerElement<Or>("OR (Bool)", ":Docs/GateIcons/OR.ico");
    registerElement<Xnor>("XNOR (Bool)", ":Docs/GateIcons/XNOR.ico");
    registerElement<Xor>("XOR (Bool)", ":Docs/GateIcons/XOR.ico");

    registerElement<Clock>("Delta Time (ms)", ":Docs/GateIcons/CLOCK.ico");

}

void Registry::addElement(MetaInfo &metaInfo)
{
    auto &metaInfos = m_pimpl->metaInfos;
    metaInfos.push_back(std::move(metaInfo));
}

Element *Registry::createElement(string::hash_t const hash)
{
    auto const &META_INFO = metaInfoFor(hash);
    assert(META_INFO.cloneElement);
    return META_INFO.cloneElement();
}

Node *Registry::createNode(string::hash_t const hash)
{
    auto const &META_INFO = metaInfoFor(hash);
    assert(META_INFO.cloneNode);
    return META_INFO.cloneNode();
}

QString Registry::elementName(string::hash_t const hash)
{
    auto const &META_INFO = metaInfoFor(hash);
    return META_INFO.name;
}

QString Registry::elementIcon(string::hash_t const hash)
{
    auto const &META_INFO = metaInfoFor(hash);
    return META_INFO.icon;
}

bool Registry::hasElement(string::hash_t const hash) const
{
    auto const &META_INFOS = m_pimpl->metaInfos;
    auto const IT = std::find_if(std::begin(META_INFOS), std::end(META_INFOS),
                                [hash](auto const &a_metaInfo) { return a_metaInfo.hash == hash; });
    return IT != std::end(META_INFOS);
}

size_t Registry::size() const
{
    auto const &META_INFOS = m_pimpl->metaInfos;
    return META_INFOS.size();
}


Registry::MetaInfo const &Registry::metaInfoFor(string::hash_t const hash) const
{
    auto &metaInfos = m_pimpl->metaInfos;
    assert(hasElement(hash));
    auto const IT = std::find_if(std::begin(metaInfos), std::end(metaInfos),
                                 [hash](auto const &metaInfo) { return metaInfo.hash == hash; });
    return *IT;
}

Registry::MetaInfo const &Registry::metaInfoAt(size_t const index) const
{
    auto const &META_INFOS = m_pimpl->metaInfos;
    return META_INFOS[index];
}
