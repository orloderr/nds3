#ifndef NDSBASEIMPL_H
#define NDSBASEIMPL_H

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "../include/nds3/definitions.h"


namespace nds
{


class NodeImpl;
class PortImpl;
class FactoryBaseImpl;

/**
 * @internal
 * @brief This is the base class for the implementation of nodes, devices, PVs.
 *
 */
class BaseImpl: public std::enable_shared_from_this<BaseImpl>
{
    friend class NodeImpl;
    friend class PortImpl;

protected:
    BaseImpl(const std::string& name);

public:
    BaseImpl(const BaseImpl& right) = delete;

    virtual ~BaseImpl(){}

    /**
     * @brief Get the Node that holds an ASYN port. Query the parent nodes if necessary.
     *
     * @return a reference to the parent AsynNode, which can be used to communicate directly with
     *         the AsynDriver
     */
    virtual std::shared_ptr<PortImpl> getPort();


    /**
     * @brief Return the node's name.
     *
     * @return the node's name.
     */
    std::string getComponentName() const;


    /**
     * @brief Return the full node's name, prepending the parents' names if necessary
     *        (e.g. "ROOT-CHANNEL1-THISNODE")
     *
     * @return the fully qualified node's name
     */
    std::string getFullName() const;

    void setParent(std::shared_ptr<NodeImpl> pParent);

    std::shared_ptr<NodeImpl> getParent() const;


    /**
     * @brief Return the node's name as seen by the parent AsynPort.
     *
     * For instance, for a record "TIME" contained in the channel "CH0" that is part of
     * an AsynPort named "DEVICE" the return value will be "CH0-TIME": node that the "DEVICE"
     * part is missing.
     *
     * @return the node's name relative to the AsynPort that contains it.
     */
    virtual std::string getFullNameFromPort() const;

    /**
     * @brief Registers all the records with the control system. Call this from the root node
     *        which will take care of traversing its children and initialize them.
     *
     * On EPICS it will create all the PVs and register them with the AsynDriver,
     * on Tango will create the dynamic attributes.
     */
    virtual void initialize(FactoryBaseImpl& controlSystem) = 0;

    /**
     * @brief Deregister all the records from the control system. Call this from the root node
     *        which will take care of traversing its children and deinitialize them.
     */
    virtual void deinitialize() = 0;

    /**
     * @brief Return the current time.
     *
     * If a delegate function has been defined with setTimestampDelegate() then call
     *  the delegated function, otherwise call the parent node's getTimestamp().
     * If the node has no parent then return the current time as reported by the
     *  operating system.
     *
     * @return the current time
     */
    timespec getTimestamp() const;

    void setTimestampDelegate(getTimestampPlugin_t timestampDelegate);

protected:
    timespec getLocalTimestamp() const;

    std::string m_name;
    std::weak_ptr<NodeImpl> m_pParent;

    getTimestampPlugin_t m_timestampFunction;

};


}










#endif // NDSBASEIMPL_H
