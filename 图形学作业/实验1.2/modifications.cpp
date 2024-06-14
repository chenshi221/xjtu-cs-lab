<<<<<<< HEAD
Matrix4f Object::model()
{
    // 创建一个单位矩阵
    Matrix4f modelMatrix = Matrix4f::Identity();

    // 将四元数转换为欧拉角(单位度）
    const Quaternionf& r             = rotation;
    auto [x_angle, y_angle, z_angle] = quaternion_to_ZYX_euler(r.w(), r.x(), r.y(), r.z());

    // 构建旋转矩阵，使用欧拉角
    Matrix4f rotationMatrix = Matrix4f::Identity();

    // 构建绕Z轴的旋转矩阵
    float cos_z = std::cos(radians(z_angle));
    float sin_z = std::sin(radians(z_angle));
    Matrix4f rotationZ;
    rotationZ << cos_z, -sin_z, 0.0f, 0.0f, sin_z, cos_z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f;

    // 构建绕Y轴的旋转矩阵
    float cos_y = std::cos(radians(y_angle));
    float sin_y = std::sin(radians(y_angle));
    Matrix4f rotationY;
    rotationY << cos_y, 0.0f, sin_y, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sin_y, 0.0f, cos_y, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f;

    // 构建绕X轴的旋转矩阵
    float cos_x = std::cos(radians(x_angle));
    float sin_x = std::sin(radians(x_angle));
    Matrix4f rotationX;
    rotationX << 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos_x, -sin_x, 0.0f, 0.0f, sin_x, cos_x, 0.0f,
        0.0f,
        0.0f, 0.0f, 1.0f;

    // 将三个旋转矩阵组合成一个4x4的旋转矩阵
    rotationMatrix = rotationX * rotationY * rotationZ;

    // 设置缩放矩阵
    Matrix4f scaleMatrix = Matrix4f::Identity();
    scaleMatrix(0, 0)    = scaling.x();
    scaleMatrix(1, 1)    = scaling.y();
    scaleMatrix(2, 2)    = scaling.z();

    // 设置位移矩阵，将物体平移到指定的位置（center）
    modelMatrix.block<3, 1>(0, 3) = center;

    // 将位移、旋转和缩放组合成模型矩阵
    modelMatrix = modelMatrix* rotationMatrix * scaleMatrix;

    return modelMatrix;
}
=======
Matrix4f Object::model()
{
    // 创建一个单位矩阵
    Matrix4f modelMatrix = Matrix4f::Identity();

    // 将四元数转换为欧拉角(单位度）
    const Quaternionf& r             = rotation;
    auto [x_angle, y_angle, z_angle] = quaternion_to_ZYX_euler(r.w(), r.x(), r.y(), r.z());

    // 构建旋转矩阵，使用欧拉角
    Matrix4f rotationMatrix = Matrix4f::Identity();

    // 构建绕Z轴的旋转矩阵
    float cos_z = std::cos(radians(z_angle));
    float sin_z = std::sin(radians(z_angle));
    Matrix4f rotationZ;
    rotationZ << cos_z, -sin_z, 0.0f, 0.0f, sin_z, cos_z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f;

    // 构建绕Y轴的旋转矩阵
    float cos_y = std::cos(radians(y_angle));
    float sin_y = std::sin(radians(y_angle));
    Matrix4f rotationY;
    rotationY << cos_y, 0.0f, sin_y, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sin_y, 0.0f, cos_y, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f;

    // 构建绕X轴的旋转矩阵
    float cos_x = std::cos(radians(x_angle));
    float sin_x = std::sin(radians(x_angle));
    Matrix4f rotationX;
    rotationX << 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos_x, -sin_x, 0.0f, 0.0f, sin_x, cos_x, 0.0f,
        0.0f,
        0.0f, 0.0f, 1.0f;

    // 将三个旋转矩阵组合成一个4x4的旋转矩阵
    rotationMatrix = rotationX * rotationY * rotationZ;

    // 设置缩放矩阵
    Matrix4f scaleMatrix = Matrix4f::Identity();
    scaleMatrix(0, 0)    = scaling.x();
    scaleMatrix(1, 1)    = scaling.y();
    scaleMatrix(2, 2)    = scaling.z();

    // 设置位移矩阵，将物体平移到指定的位置（center）
    modelMatrix.block<3, 1>(0, 3) = center;

    // 将位移、旋转和缩放组合成模型矩阵
    modelMatrix = modelMatrix* rotationMatrix * scaleMatrix;

    return modelMatrix;
}
>>>>>>> master
