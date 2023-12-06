void HalfedgeMesh::loop_subdivide()
{
    optional<HalfedgeMeshFailure> check_result = validate();
    if (check_result.has_value()) {
        return;
    }
    logger->info("subdivide object {} (ID: {}) with Loop Subdivision strategy", object.name,
                 object.id);
    logger->info("original mesh: {} vertices, {} faces in total, {} edges in total", vertices.size,
                 faces.size, edges.size);

    // 使用 Loop 细分规则计算输入网格中所有顶点的新位置，并将它们存储在 Vertex::new_pos 中。

    for (Vertex* v = vertices.head; v != nullptr; v = v->next_node) {
        int num      = 0;
        Vector3f sum = Vector3f(0.0f, 0.0f, 0.0f);
        Halfedge* h  = v->halfedge;
        do {
            sum += h->inv->from->pos;
            h = h->prev->inv;
            num++;
        } while (h != v->halfedge);
        if (num <= 3) {
            v->new_pos = (1 - 3.0f * 3.0f / 16.0f) * v->pos + 3.0f / 16.0f * sum;
        } else {
            float u    = 3.0f / (8.0f * num);
            float w    = 1.0f - num * u;
            v->new_pos = u * sum + w * v->pos;
        }
    }

    // 计算与边关联的细分后顶点位置，并将它们存储在 Edge::new_pos 中。

    for (Edge* e = edges.head; e != nullptr; e = e->next_node) {
        e->new_pos =
            3.0f / 8.0f * (e->halfedge->from->pos + e->halfedge->inv->from->pos) +
            1.0f / 8.0f *
                (e->halfedge->next->next->from->pos + e->halfedge->inv->next->next->from->pos);
    }

    // 更新旧顶点位置

    for (Vertex* v = vertices.head; v != nullptr; v = v->next_node) {
        v->pos = v->new_pos;
    }

    // 记录原始链表的最后一位

    Vertex* last_vertex = vertices.tail;
    Edge* last_edge     = edges.tail;

    // 分裂每条边直到原始最后一条边

    for (Edge* e = edges.head; e != last_edge->next_node;) {
        // split_edge(e)会销毁e，所以要先记录下一个边
        Edge* temp_e      = e->next_node;
        Vector3f temp_pos = e->new_pos;
        // 分裂
        std::optional<Vertex*> v_opt = split_edge(e);
        // 设置新坐标
        v_opt.value()->pos = temp_pos;
        // 区分通过分割原始网格中的边而产生的细分后边与新边，通过设置 Edge::is_new 属性来实现。
        v_opt.value()->halfedge->edge->is_new                  = false;
        v_opt.value()->halfedge->prev->edge->is_new            = true;
        v_opt.value()->halfedge->prev->inv->prev->edge->is_new = false;
        v_opt.value()->halfedge->inv->next->edge->is_new       = true;

        e = temp_e;
    }

    // 标记新建的顶点

    for (Vertex* v = last_vertex; v != nullptr; v = v->next_node) {
        v->is_new = true;
    }

    // 旋转新建的边，被旋转的边有个特点，它一定是一端链接者一个新建的顶点，另一端链接着一个原始顶点
    // 同时需要旋转的边被另外两条新建的边包围着，所以我们遍历所有新建的顶点，然后遍历它的所有边，找到符合条件的边

    for (Vertex* v = last_vertex; v != nullptr; v = v->next_node) {
        // 先找到一个half_new作为起点
        Halfedge* h = v->halfedge;
        while (h->edge->is_new) {
            h = h->prev->inv;
        }
        Halfedge* h_start = h;
        h                 = h->prev->inv;
        while (h != h_start) {
            if (h->edge->is_new && h->prev->edge->is_new && h->prev->inv->prev->edge->is_new) {
                Halfedge* temp = h->prev->inv->prev->inv;
                flip_edge(h->prev->edge);
                h = temp;
            } else {
                h = h->prev->inv;
            }
        }
    }

    // 将所有新建的顶点的is_new属性设置为false

    for (Vertex* v = vertices.head; v != nullptr; v = v->next_node) {
        v->is_new = false;
    }

    // 将所有的边的is_new属性设置为false

    for (Edge* e = edges.head; e != nullptr; e = e->next_node) {
        e->is_new = false;
    }

    global_inconsistent = true;
    logger->info("original mesh: {} vertices, {} faces in total, {} edges in total", vertices.size,
                 faces.size, edges.size);
    logger->info("Loop Subdivision done");
    logger->info("");
    validate();
}