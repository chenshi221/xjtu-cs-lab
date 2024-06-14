<<<<<<< HEAD
void Scene::simulation_update()
{
    // 这次模拟的总时长不是上一帧的时长，而是上一帧时长与之前帧剩余时长的总和，
    // 即上次调用 simulation_update 到现在过了多久。
    time_point now = steady_clock::now();
    duration dura  = now - last_update;

    // 以固定的时间步长 (time_step) 循环模拟物体运动，每模拟一步，模拟总时长就减去一个
    // time_step ，当总时长不够一个 time_step 时停止模拟。
    int num = 0;
    while (dura > duration(time_step)) {
        selected_object->update(all_objects);
        dura= dura - duration(time_step);
        num++;
    }

    // 根据刚才模拟时间步的数量，更新最后一次调用 simulation_update 的时间 (last_update)。
    last_update =now - std::chrono::duration_cast<std::chrono::steady_clock::duration>(dura);
}


// Function to perform a single Forward Euler step
KineticState forward_euler_step([[maybe_unused]] const KineticState& previous,
                                const KineticState& current)
{
    // 使用欧拉前向积分计算下一个状态
    Vector3f new_velocity = current.velocity + current.acceleration * time_step;
    Vector3f new_position = current.position + current.velocity * time_step;

    // 创建并返回新的状态
    KineticState next(new_position, new_velocity, current.acceleration);
    return next;
}

void Object::update(vector<Object*>& all_objects)
{
    // 首先调用 step 函数计下一步该物体的运动学状态。
    KineticState current_state{center, velocity, force / mass};
    KineticState next_state = step(prev_state, current_state);
    
    // 将物体的位置移动到下一步状态处，但暂时不要修改物体的速度。
    center = next_state.position;
    // 遍历 all_objects，检查该物体在下一步状态的位置处是否会与其他物体发生碰撞。
    for (auto object : all_objects) {
        (void)object;

        // 检测该物体与另一物体是否碰撞的方法是：
        // 遍历该物体的每一条边，构造与边重合的射线去和另一物体求交，如果求交结果非空、
        // 相交处也在这条边的两个端点之间，那么该物体与另一物体发生碰撞。
        // 请时刻注意：物体 mesh 顶点的坐标都在模型坐标系下，你需要先将其变换到世界坐标系。
        for (size_t i = 0; i < mesh.edges.count(); ++i) {
            array<size_t, 2> v_indices = mesh.edge(i);
            (void)v_indices;
            // v_indices 中是这条边两个端点的索引，以这两个索引为参数调用 GL::Mesh::vertex
            // 方法可以获得它们的坐标，进而用于构造射线。
            if (BVH_for_collision) {
            } else {
            }
            // 根据求交结果，判断该物体与另一物体是否发生了碰撞。
            // 如果发生碰撞，按动量定理计算两个物体碰撞后的速度，并将下一步状态的位置设为
            // current_state.position ，以避免重复碰撞。
        }
    }
    // 将上一步状态赋值为当前状态，并将物体更新到下一步状态。
    prev_state = current_state;
    center     = next_state.position;
    velocity   = next_state.velocity;
    force      = next_state.acceleration * mass;

=======
void Scene::simulation_update()
{
    // 这次模拟的总时长不是上一帧的时长，而是上一帧时长与之前帧剩余时长的总和，
    // 即上次调用 simulation_update 到现在过了多久。
    time_point now = steady_clock::now();
    duration dura  = now - last_update;

    // 以固定的时间步长 (time_step) 循环模拟物体运动，每模拟一步，模拟总时长就减去一个
    // time_step ，当总时长不够一个 time_step 时停止模拟。
    int num = 0;
    while (dura > duration(time_step)) {
        selected_object->update(all_objects);
        dura= dura - duration(time_step);
        num++;
    }

    // 根据刚才模拟时间步的数量，更新最后一次调用 simulation_update 的时间 (last_update)。
    last_update =now - std::chrono::duration_cast<std::chrono::steady_clock::duration>(dura);
}


// Function to perform a single Forward Euler step
KineticState forward_euler_step([[maybe_unused]] const KineticState& previous,
                                const KineticState& current)
{
    // 使用欧拉前向积分计算下一个状态
    Vector3f new_velocity = current.velocity + current.acceleration * time_step;
    Vector3f new_position = current.position + current.velocity * time_step;

    // 创建并返回新的状态
    KineticState next(new_position, new_velocity, current.acceleration);
    return next;
}

void Object::update(vector<Object*>& all_objects)
{
    // 首先调用 step 函数计下一步该物体的运动学状态。
    KineticState current_state{center, velocity, force / mass};
    KineticState next_state = step(prev_state, current_state);
    
    // 将物体的位置移动到下一步状态处，但暂时不要修改物体的速度。
    center = next_state.position;
    // 遍历 all_objects，检查该物体在下一步状态的位置处是否会与其他物体发生碰撞。
    for (auto object : all_objects) {
        (void)object;

        // 检测该物体与另一物体是否碰撞的方法是：
        // 遍历该物体的每一条边，构造与边重合的射线去和另一物体求交，如果求交结果非空、
        // 相交处也在这条边的两个端点之间，那么该物体与另一物体发生碰撞。
        // 请时刻注意：物体 mesh 顶点的坐标都在模型坐标系下，你需要先将其变换到世界坐标系。
        for (size_t i = 0; i < mesh.edges.count(); ++i) {
            array<size_t, 2> v_indices = mesh.edge(i);
            (void)v_indices;
            // v_indices 中是这条边两个端点的索引，以这两个索引为参数调用 GL::Mesh::vertex
            // 方法可以获得它们的坐标，进而用于构造射线。
            if (BVH_for_collision) {
            } else {
            }
            // 根据求交结果，判断该物体与另一物体是否发生了碰撞。
            // 如果发生碰撞，按动量定理计算两个物体碰撞后的速度，并将下一步状态的位置设为
            // current_state.position ，以避免重复碰撞。
        }
    }
    // 将上一步状态赋值为当前状态，并将物体更新到下一步状态。
    prev_state = current_state;
    center     = next_state.position;
    velocity   = next_state.velocity;
    force      = next_state.acceleration * mass;

>>>>>>> master
}