//
// Created by airlab on 7/7/23.
//

#ifndef BT_MANIPULATOR_DEMO_MANIPULATOR_NODE_H
#define BT_MANIPULATOR_DEMO_MANIPULATOR_NODE_H
#include <chrono>
#include "behaviortree_cpp/behavior_tree.h"
#include "behaviortree_cpp/bt_factory.h"
#include "MyCobot.hpp"

#define SOLENOID_PIN 21
#define VACUUM_PIN 20
#define WAIT_TIME 2


namespace ManipulatorNode {

    using BT::NodeStatus;
    using namespace std::chrono_literals;

    BT::NodeStatus IsControllerConnected();
    BT::NodeStatus IsPowerOn();

    class SuctionInterface
    {
    public:
        SuctionInterface() : _on(true)
        {
        }

        NodeStatus on();

        NodeStatus off();

    private:
        bool _on;
    };

    class ApproachObject : public BT::SyncActionNode
    {
    public:
        ApproachObject(const std::string& name) :
                BT::SyncActionNode(name, {})
        {
        }

        // You must override the virtual function tick()
        NodeStatus tick() override;
    };

    // with an input port.
    class MissionCoordinator : public BT::SyncActionNode
    {
    public:
        MissionCoordinator(const std::string& name, const BT::NodeConfig& config)
                : BT::SyncActionNode(name, config)
        {
        }

        // You must override the virtual function tick()
        NodeStatus tick() override;

        // It is mandatory to define this static method.
        static BT::PortsList providedPorts()
        {
            return{ BT::InputPort<std::string>("message") };
        }
    };

    class Rest : public BT::StatefulActionNode
    {
    public:
        Rest(const std::string& name, const BT::NodeConfig& config)
                : BT::StatefulActionNode(name, config)
        {}

        static BT::PortsList providedPorts()
        {
            // amount of milliseconds that we want to sleep
            return{ BT::InputPort<int>("msec") };
        }

        NodeStatus onStart() override
        {
            int msec = 0;
            getInput("msec", msec);
            if( msec <= 0 )
            {
                // no need to go into the RUNNING state
                return NodeStatus::SUCCESS;
            }
            else {
                using namespace std::chrono;
                // once the deadline is reached, we will return SUCCESS.
                deadline_ = system_clock::now() + milliseconds(msec);
                return NodeStatus::RUNNING;
            }
        }

        /// method invoked by an action in the RUNNING state.
        NodeStatus onRunning() override
        {
            if ( std::chrono::system_clock::now() >= deadline_ )
            {
                return NodeStatus::SUCCESS;
            }
            else {
                return NodeStatus::RUNNING;
            }
        }

        void onHalted() override
        {
            // nothing to do here...
            std::cout << "SleepNode interrupted" << std::endl;
        }

    private:
        std::chrono::system_clock::time_point deadline_;
    };


    inline void RegisterNodes(BT::BehaviorTreeFactory& factory)
    {
        static SuctionInterface suction_singleton;

        factory.registerSimpleCondition("IsControllerConnected", std::bind(IsControllerConnected));
        factory.registerSimpleCondition("IsPowerOn", std::bind(IsPowerOn));
        factory.registerSimpleAction("OnSuction", std::bind(&SuctionInterface::on, &suction_singleton));
        factory.registerSimpleAction("OffSuction", std::bind(&SuctionInterface::off, &suction_singleton));
        factory.registerNodeType<ApproachObject>("ApproachObject");
        factory.registerNodeType<MissionCoordinator>("MissionCoordinator");
    }

}


#endif //BT_MANIPULATOR_DEMO_MANIPULATOR_NODE_H
