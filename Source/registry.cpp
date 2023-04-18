#include "Include/registry.h"

#include "Include/Elements/all.h"
#include "Include/Nodes/all.h"
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
    registerInternalElements();
}

void Registry::registerInternalElements()
{
    init_resources();

    registerElement<Package, nodes::NodePackage>("Package", "");

    using namespace elements;

    registerElement<gates::And>("AND", ":Docs/GateIcons/AND.ico");
    registerElement<gates::Nand>("NAND", ":Docs/GateIcons/NAND.ico");
    registerElement<gates::Nor>("NOR", ":Docs/GateIcons/NOR.ico");
    registerElement<gates::Not>("NOT", ":Docs/GateIcons/NOT.ico");
    registerElement<gates::Or>("OR", ":Docs/GateIcons/OR.ico");
    registerElement<gates::Xnor>("XNOR", ":Docs/GateIcons/XNOR.ico");
    registerElement<gates::Xor>("XOR", ":Docs/GateIcons/XOR.ico");

    registerElement<timers::Clock, nodes::timers::Clock>("CLOCK", ":Docs/GateIcons/CLOCK.ico");

    registerElement<logic::Multiplexer>("Multiplexer", ":Docs/LogicIcons/Multiplexer.ico");
    registerElement<logic::Demultiplexer>("Demultiplexer", ":Docs/LogicIcons/Demultiplexer.ico");

    registerElement<Ui::IntInfo, nodes::Ui::IntInfo>("Output (Int)", ":Docs/UiIcons/const_int.png");
    registerElement<Ui::ToggleButton, nodes::Ui::ToggleButton>("Toggle Button", ":Docs/UiIcons/toggle_button.png");
    //registerElement<Ui::SevenSegmentDisplay, nodes::Ui::SevenSegmentDisplay>("7 Segment Display", ":Docs/UiIcons/7_segment_display.png");
    //registerElement<Ui::BCDToSevenSegmentDisplay>("BCD To Seven Segment Display", ":Docs/UiIcons/BCD_to_7_segment.png");
    registerElement<Ui::PushButton, nodes::Ui::PushButton>("Push Button", ":Docs/UiIcons/push_button.png");

    //registerElement<values::ConstBool, nodes::values::ConstBool>("Boolean", ":Docs/UiIcons/const_bool.png");
    registerElement<values::ConstInt, nodes::values::ConstInt>("Input (Int)", ":Docs/UiIcons/const_int.png");
    registerElement<values::BCD>("BCD", ":Docs/UiIcons/BCD.png");

    registerElement<values::OutputBool, nodes::values::OutputBool>("Output (Bool)", ":Docs/UiIcons/const_bool.png");

    registerElement<values::RandomBool>("Trigger (Random Bool)", ":Docs/UiIcons/random_bool.png");
    //registerElement<logic::Switch, nodes::logic::Switch>("Switch (Int)", ":/logic/switch.png");

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
