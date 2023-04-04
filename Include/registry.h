#ifndef REGISTRY_H
#define REGISTRY_H

//#include <cstdint>
#include <QString>


#include "Include/strings.h"

class Element;
class Node;

class Registry
{
    struct MetaInfo {
        string::hash_t hash{};
        QString type{};
        QString name{};
        QString icon{};
        template<typename T>
        using CloneFunc = T *(*)();
        CloneFunc<Element> cloneElement{};
        CloneFunc<Node> cloneNode{};
    };

public:

    /*struct PackageInfo {
      std::string filename{};
      std::string path{};
      std::string icon{};
    };
    using Packages = std::unordered_map<std::string, PackageInfo>; */

    static Registry &get();
    ~Registry() = default;
    void registerInternalElements();

    template<typename ElementDerived, typename NodeDerived = Node>
    typename std::enable_if_t<(std::is_base_of_v<Element, ElementDerived> && std::is_base_of_v<Node, NodeDerived>)>
    registerElement(QString name, QString icon)
    {
        string::hash_t const hash{ ElementDerived::HASH };
        assert(!hasElement(hash));
        MetaInfo info{ hash,
                     ElementDerived::TYPE,
                     std::move(name),
                     std::move(icon),
                     &cloneElement<ElementDerived>,
                     &cloneNode<NodeDerived> };
        addElement(info);
    }

    Element *createElement(char const *const name) { return createElement(string::hash(name)); }
    Element *createElement(string::hash_t const hash);

    Node *createNode(char const *const name) { return createNode(string::hash(name)); }
    Node *createNode(string::hash_t const hash);

    QString elementName(char const *const name) { return elementName(string::hash(name)); }
    QString elementName(string::hash_t const hash);

    QString elementIcon(char const *const name) { return elementIcon(string::hash(name)); }
    QString elementIcon(string::hash_t const hash);

    size_t size() const;
    MetaInfo const &metaInfoFor(string::hash_t const hash) const;
    MetaInfo const &metaInfoAt(size_t const index) const;

    bool hasElement(string::hash_t const hash) const;
    //Packages const &packages() const;

private:
    Registry();

    void addElement(MetaInfo &metaInfo);

    template<typename T>
    static Element *cloneElement()
    {
      return new T;
    }

    template<typename T>
    static Node *cloneNode()
    {
      return new T;
    }

private:
    struct PIMPL;
    std::unique_ptr<PIMPL> m_pimpl;
};

#endif // REGISTRY_H
