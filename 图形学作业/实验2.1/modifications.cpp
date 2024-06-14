<<<<<<< HEAD
void Toolbar::layout_mode(Scene& scene)
{
    if (ImGui::BeginTabItem("Layout")) {
        if (mode != WorkingMode::LAYOUT) {
            on_selection_canceled();
            mode = WorkingMode::LAYOUT;
        }
        scene_hierarchies(scene);

        Object* selected_object = scene.selected_object;
        if (selected_object != nullptr) {
            material_editor(selected_object->mesh.material);
            ImGui::SeparatorText("Transform");
            ImGui::Text("Translation");
            ImGui::PushID("Translation##");
            Vector3f& center = selected_object->center;
            xyz_drag(&center.x(), &center.y(), &center.z(), POSITION_UNIT);
            ImGui::PopID();

            ImGui::Text("Scaling");
            ImGui::PushID("Scaling##");
            Vector3f& scaling = selected_object->scaling;
            xyz_drag(&scaling.x(), &scaling.y(), &scaling.z(), SCALING_UNIT);
            ImGui::PopID();

            const Quaternionf& r             = selected_object->rotation;
            auto [x_angle, y_angle, z_angle] = quaternion_to_ZYX_euler(r.w(), r.x(), r.y(), r.z());
            ImGui::Text("Rotation (ZYX Euler)");
            ImGui::PushID("Rotation##");
            ImGui::PushItemWidth(0.3f * ImGui::CalcItemWidth());
            ImGui::DragFloat("pitch", &x_angle, ANGLE_UNIT, -180.0f, 180.0f, "%.1f deg",
                             ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine();
            ImGui::DragFloat("yaw", &y_angle, ANGLE_UNIT, -90.0f, 90.0f, "%.1f deg",
                             ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine();
            ImGui::DragFloat("roll", &z_angle, ANGLE_UNIT, -180.0f, 180.0f, "%.1f deg",
                             ImGuiSliderFlags_AlwaysClamp);
            ImGui::PopItemWidth();
            ImGui::PopID();
            selected_object->rotation =
                Quaternionf(std::cos(radians(x_angle / 2)), std::sin(radians(x_angle / 2)), 0, 0) *
                Quaternionf(std::cos(radians(y_angle / 2)), 0, std::sin(radians(y_angle / 2)), 0) *
                Quaternionf(std::cos(radians(z_angle / 2)), 0, 0, std::sin(radians(z_angle / 2)));
        }
        ImGui::EndTabItem();
    }
=======
void Toolbar::layout_mode(Scene& scene)
{
    if (ImGui::BeginTabItem("Layout")) {
        if (mode != WorkingMode::LAYOUT) {
            on_selection_canceled();
            mode = WorkingMode::LAYOUT;
        }
        scene_hierarchies(scene);

        Object* selected_object = scene.selected_object;
        if (selected_object != nullptr) {
            material_editor(selected_object->mesh.material);
            ImGui::SeparatorText("Transform");
            ImGui::Text("Translation");
            ImGui::PushID("Translation##");
            Vector3f& center = selected_object->center;
            xyz_drag(&center.x(), &center.y(), &center.z(), POSITION_UNIT);
            ImGui::PopID();

            ImGui::Text("Scaling");
            ImGui::PushID("Scaling##");
            Vector3f& scaling = selected_object->scaling;
            xyz_drag(&scaling.x(), &scaling.y(), &scaling.z(), SCALING_UNIT);
            ImGui::PopID();

            const Quaternionf& r             = selected_object->rotation;
            auto [x_angle, y_angle, z_angle] = quaternion_to_ZYX_euler(r.w(), r.x(), r.y(), r.z());
            ImGui::Text("Rotation (ZYX Euler)");
            ImGui::PushID("Rotation##");
            ImGui::PushItemWidth(0.3f * ImGui::CalcItemWidth());
            ImGui::DragFloat("pitch", &x_angle, ANGLE_UNIT, -180.0f, 180.0f, "%.1f deg",
                             ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine();
            ImGui::DragFloat("yaw", &y_angle, ANGLE_UNIT, -90.0f, 90.0f, "%.1f deg",
                             ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine();
            ImGui::DragFloat("roll", &z_angle, ANGLE_UNIT, -180.0f, 180.0f, "%.1f deg",
                             ImGuiSliderFlags_AlwaysClamp);
            ImGui::PopItemWidth();
            ImGui::PopID();
            selected_object->rotation =
                Quaternionf(std::cos(radians(x_angle / 2)), std::sin(radians(x_angle / 2)), 0, 0) *
                Quaternionf(std::cos(radians(y_angle / 2)), 0, std::sin(radians(y_angle / 2)), 0) *
                Quaternionf(std::cos(radians(z_angle / 2)), 0, 0, std::sin(radians(z_angle / 2)));
        }
        ImGui::EndTabItem();
    }
>>>>>>> master
}