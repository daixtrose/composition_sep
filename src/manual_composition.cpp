// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>

#include "client_component/client_component.hpp"
#include "listener_component/listener_component.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rxcpp/rx.hpp"
#include "server_component/server_component.hpp"
#include "talker_component/talker_component.hpp"

#include <algorithm>
#include <chrono>
#include <thread>

int main(int argc, char* argv[])
{
    // Force flush of the stdout buffer.
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    // Initialize any global resources needed by the middleware and the client
    // library. This will also parse command line arguments one day (as of Beta
    // 1 they are not used). You must call this before using any other part of
    // the ROS system. This should be called once per process.
    rclcpp::init(argc, argv);

    // Create an executor that will be responsible for execution of callbacks
    // for a set of nodes. With this version, all callbacks will be called from
    // within this thread (the main one).
    rclcpp::executors::MultiThreadedExecutor exec;
    rclcpp::NodeOptions options;

    // Add some nodes to the executor which provide work for the executor during
    // its "spin" function. An example of available work is executing a
    // subscription callback, or a timer callback.
    auto talker = std::make_shared<composition::Talker>(options);
    exec.add_node(talker);

    using string_t = std_msgs::msg::String;
    std::vector<string_t> receivedMessages;

    auto reaction = [&receivedMessages](auto msg) {
        std::cerr << "Received " << msg.data;
        std::cerr << " in thread " << std::this_thread::get_id() << std::endl;
        receivedMessages.push_back(msg);
    };

    auto listener = std::make_shared<composition::Listener>(options, reaction);
    exec.add_node(listener);

    // Here we deviate from the toy example
    std::thread t([&exec]() {
        std::cerr << "Starting to spin" << std::endl;
        exec.spin();
    });

    using namespace std::chrono_literals;
    // std::this_thread::sleep_for(1s);

    auto makeMsg = [](auto s) {
        string_t result;
        result.data = std::move(s);
        return result;
    };

    std::vector<string_t> sentMessages
        = { makeMsg("A"), makeMsg("B"), makeMsg("C") };

    auto task = [&sentMessages, &talker]() {
        for (auto const& msg : sentMessages) {
            std::cerr << "Publishing " << msg.data;
            std::cerr << " in thread " << std::this_thread::get_id()
                      << std::endl;
            talker->publish(msg);
            std::this_thread::sleep_for(1ms);
        }
    };

    std::thread t2(task);
    std::this_thread::sleep_for(2s);

    exec.cancel();
    t.join();
    t2.join();
    rclcpp::shutdown();

    std::cerr << "receivedMessages = ";
    for (auto msg : receivedMessages) {
        std::cerr << msg.data << ", ";
    }

    return 0;
}

/*
    // register at the Rx interface of the listener
    std::vector<string_t> receivedMessages;

    auto reaction = [&receivedMessages](auto msg) {
        std::cerr << "Received " << msg.data;
        std::cerr << " in thread " << std::this_thread::get_id() << std::endl;
        receivedMessages.push_back(msg);
    };

    auto subscription = listener->observable().subscribe(reaction);

    std::cerr << "Starting to spin" << std::endl;
    std::thread t([&exec]() { exec.spin(); });

    auto makeMsg = [](auto s) {
        string_t result;
        result.data = std::move(s);
        return result;
    };

    std::vector<string_t> sentMessages
        = { makeMsg("A"), makeMsg("B"), makeMsg("C") };

    using namespace std::chrono_literals;

    auto task = [&sentMessages, &talker]() {
        for (auto const& msg : sentMessages) {
            std::cerr << "Publishing " << msg.data;
            std::cerr << " in thread " << std::this_thread::get_id()
                      << std::endl;
            talker->publish(msg);
            std::this_thread::sleep_for(1ms);
        }
    };

    std::thread t2([&]() { task(); });
    std::this_thread::sleep_for(2s);

    // auto taskRepeater = duckbrain_ros2_utilities::makeTaskRepeater<string_t>(
    //     "task_repeater", options, 1s, task);

    // exec.add_node(taskRepeater);

    // exec.spin();

    exec.cancel();
    t.join();
    t2.join();
    rclcpp::shutdown();

    REQUIRE(receivedMessages == sentMessages);

*/