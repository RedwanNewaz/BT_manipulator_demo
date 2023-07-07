//
// Created by airlab on 7/7/23.
//
#include "behaviortree_cpp/bt_factory.h"
#include <iostream>
#include "src/manipulator_node.h"
#include "src/movebase_node.h"



using namespace BT;


//ReactiveSequence
static const char* xml_text_reactive = R"(

 <root BTCPP_format="4" main_tree_to_execute="MainTree">

     <BehaviorTree ID="MainTree">
        <ReactiveSequence name="root">
            <IsControllerConnected/>
            <IsPowerOn/>
            <Sequence>
                <MissionCoordinator   message="mission started..." />
                <MoveBase       goal="1;2;3"/>
                <Rest          msec="5000"/>
                <SubTree ID="Subtree" name="PickObject"/>
                <Rest          msec="2000"/>
                <MoveBase       goal="1;2;3"/>
                <Rest          msec="2000"/>
                <OffSuction/>
                <MissionCoordinator   message="mission completed!" />
            </Sequence>
        </ReactiveSequence>
     </BehaviorTree>

    <BehaviorTree ID="Subtree">
        <Sequence name="PickApproach">
            <RetryUntilSuccessful num_attempts="3">
                <Sequence name="PickObject">
                    <ApproachObject />
                    <Rest          msec="2000"/>
                    <OnSuction/>
                </Sequence>
            </RetryUntilSuccessful>
        </Sequence>
    </BehaviorTree>

 </root>
 )";


// clang-format on

using namespace ManipulatorNode;

int main()
{
    BehaviorTreeFactory factory;

    factory.registerNodeType<MoveBaseAction>("MoveBase");

    static SuctionInterface suction_singleton;
    factory.registerSimpleCondition("IsControllerConnected", std::bind(IsControllerConnected));
    factory.registerSimpleCondition("IsPowerOn", std::bind(IsPowerOn));
    factory.registerSimpleAction("OnSuction", std::bind(&SuctionInterface::on, &suction_singleton));
    factory.registerSimpleAction("OffSuction", std::bind(&SuctionInterface::off, &suction_singleton));
    factory.registerNodeType<ApproachObject>("ApproachObject");
    factory.registerNodeType<MissionCoordinator>("MissionCoordinator");
    factory.registerNodeType<Rest>("Rest");


    for (auto& xml_text : { xml_text_reactive})
    {
        std::cout << "\n------------ BUILDING A NEW TREE ------------\n\n";

        auto tree = factory.createTreeFromText(xml_text);

        NodeStatus status = NodeStatus::IDLE;

        // If we need to run code between one tick() and the next,
        // we can implement our own while loop
        while (status != NodeStatus::SUCCESS)
        {
            std::cout << "--- ticking\n";
            status = tree.tickOnce();
            std::cout << "--- status: " << toStr(status) << "\n\n";

            // if still running, add some wait time
            if (status == NodeStatus::RUNNING)
            {
                tree.sleep(std::chrono::milliseconds(100));
            }
        }
    }
    return 0;
}