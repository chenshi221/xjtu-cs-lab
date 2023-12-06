Matrix4f Camera::projection()
{
    const float fov_y = radians(fov_y_degrees);

    Matrix4f projection = Matrix4f::Zero();
    // 使用平行投影时，用户并不能从画面上直观地感受到相机的位置，
    // 因而会产生近处物体裁剪过多的错觉。为了产程更好的观察效果，
    // 这里没有使用相机本身的 near 而是取 near = -far 来让相机能看到“背后”的物体。
    projection(0, 0) = 1.0f / (std::tan(fov_y / 2) * aspect_ratio);
    projection(1, 1) = 1.0f / std::tan(fov_y / 2);
    projection(2, 2) = -(far + near) / (far - near);
    projection(2, 3) = -2 * far * near / (far - near);
    projection(3, 2) = -1.0f;

    return projection;
}
