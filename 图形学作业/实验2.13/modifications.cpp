// Function to perform a single Runge-Kutta step
KineticState runge_kutta_step([[maybe_unused]] const KineticState& previous,
                              const KineticState& current)
{
    // Runge-Kutta coefficients
    const float dt      = time_step; // 时间步长

    // Intermediate steps
    Vector3f k1v = current.acceleration * dt; // 计算 k1 的速度增量
    Vector3f k1p = current.velocity * dt;     // 计算 k1 的位置增量

    Vector3f k2v = (current.acceleration + k1v * 0.5f) * dt; // 计算 k2 的速度增量
    Vector3f k2p = (current.velocity + k1p * 0.5f) * dt;     // 计算 k2 的位置增量

    Vector3f k3v = (current.acceleration + k2v * 0.5f) * dt; // 计算 k3 的速度增量
    Vector3f k3p = (current.velocity + k2p * 0.5f) * dt;     // 计算 k3 的位置增量

    Vector3f k4v = (current.acceleration + k3v) * dt; // 计算 k4 的速度增量
    Vector3f k4p = (current.velocity + k3p) * dt;     // 计算 k4 的位置增量

    // Final state
    Vector3f new_velocity = current.velocity + (k1v + 2.0f * k2v + 2.0f * k3v + k4v) / 6.0f;
    Vector3f new_position = current.position + (k1p + 2.0f * k2p + 2.0f * k3p + k4p) / 6.0f;

    KineticState next(new_position, new_velocity, current.acceleration);
    return next;
}

// Function to perform a single Backward Euler step
KineticState backward_euler_step([[maybe_unused]] const KineticState& previous,
                                 const KineticState& current)
{
    Vector3f new_velocity = current.velocity + current.acceleration * time_step;
    Vector3f new_position = current.position + new_velocity * time_step;

    KineticState next(new_position, new_velocity, current.acceleration);
    return next;
}

// Function to perform a single Symplectic Euler step
KineticState symplectic_euler_step([[maybe_unused]] const KineticState& previous,
                                   const KineticState& current)
{
    Vector3f new_velocity = current.velocity + current.acceleration * time_step;
    Vector3f new_position = current.position + new_velocity * time_step;

    KineticState next(new_position, new_velocity, current.acceleration);
    return next;
}
