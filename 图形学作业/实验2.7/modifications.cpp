<<<<<<< HEAD
optional<Edge*> HalfedgeMesh::flip_edge(Edge* e)
{
    // 非边界边
    if (!e->on_boundary()) {
        // 要用到的半边
        Halfedge* h     = e->halfedge;
        Halfedge* h_inv = h->inv;
        Halfedge* h_2_3 = h->next;
        Halfedge* h_3_1 = h_2_3->next;
        Halfedge* h_1_4 = h_inv->next;
        Halfedge* h_4_2 = h_1_4->next;
        // v3 and v4 are vertices opposite the edge
        Vertex* v1 = h->from;
        Vertex* v2 = h_inv->from;
        Vertex* v3 = h_3_1->from;
        Vertex* v4 = h_4_2->from;
        // 要用到的面片
        Face* f1 = h->face;
        Face* f2 = h_inv->face;
        // 重新连接各基本元素
        // 设置h
        h->next = h_3_1;
        h->prev = h_1_4;
        h->from = v4;
        // 设置h->inv
        h_inv->next = h_4_2;
        h_inv->prev = h_2_3;
        h_inv->from = v3;
        // 设置两个面绑定的半边
        f1->halfedge = h;
        f2->halfedge = h_inv;
        // 设置两个顶点绑定的半边
        v1->halfedge = h_1_4;
        v2->halfedge = h_2_3;
        // 更新外边
        h_2_3->set_neighbors(h_inv, h_4_2, h_2_3->inv, h_2_3->from, h_2_3->edge, f2);
        h_4_2->set_neighbors(h_2_3, h_inv, h_4_2->inv, h_4_2->from, h_4_2->edge, f2);
        h_3_1->set_neighbors(h_1_4, h, h_3_1->inv, h_3_1->from, h_3_1->edge, f1);
        h_1_4->set_neighbors(h, h_3_1, h_1_4->inv, h_1_4->from, h_1_4->edge, f1);

        return e;
    }

    else {
        return std::nullopt;
    }
}

optional<Vertex*> HalfedgeMesh::split_edge(Edge* e)
{
    if (!e->on_boundary()) {
        // 获取4个顶点
        Vertex* v1 = e->halfedge->from;
        Vertex* v2 = e->halfedge->inv->from;
        Vertex* v3 = e->halfedge->prev->from;
        Vertex* v4 = e->halfedge->inv->prev->from;

        // 获取边
        Halfedge* h     = e->halfedge;
        Halfedge* h_inv = e->halfedge->inv;
        Halfedge* h23   = h->next;
        Halfedge* h31   = h->prev;
        Halfedge* h42   = h_inv->prev;
        Halfedge* h14   = h_inv->next;

        // 获取面
        Face* f01 = h23->face;
        Face* f02 = h42->face;

        // 创建一个新的顶点，作为分割点
        Vertex* v5 = new_vertex();
        v5->pos    = (v2->pos + v1->pos) / 2;

        // 以边上四边创建4个面
        Face* f1     = new_face();
        f1->halfedge = h14;
        Face* f2     = new_face();
        f2->halfedge = h23;
        Face* f3     = new_face();
        f3->halfedge = h31;
        Face* f4     = new_face();
        f4->halfedge = h42;

        // 创建半边
        Halfedge* h15 = new_halfedge();
        Halfedge* h51 = new_halfedge();
        Halfedge* h25 = new_halfedge();
        Halfedge* h52 = new_halfedge();
        Halfedge* h35 = new_halfedge();
        Halfedge* h53 = new_halfedge();
        Halfedge* h45 = new_halfedge();
        Halfedge* h54 = new_halfedge();

        // 创建边
        Edge* e1     = new_edge();
        e1->halfedge = h15;
        Edge* e2     = new_edge();
        e2->halfedge = h25;
        Edge* e3     = new_edge();
        e3->halfedge = h35;
        Edge* e4     = new_edge();
        e4->halfedge = h45;

        // 设置半边
        h15->set_neighbors(h53, h31, h51, v1, e1, f3);
        h51->set_neighbors(h14, h45, h15, v5, e1, f1);
        h25->set_neighbors(h54, h42, h52, v2, e2, f4);
        h52->set_neighbors(h23, h35, h25, v5, e2, f2);
        h35->set_neighbors(h52, h23, h53, v3, e3, f2);
        h53->set_neighbors(h31, h15, h35, v5, e3, f3);
        h45->set_neighbors(h51, h14, h54, v4, e4, f1);
        h54->set_neighbors(h42, h25, h45, v5, e4, f4);

        // 更新周围4边
        h23->set_neighbors(h35, h52, h23->inv, v2, h23->edge, f2);
        h31->set_neighbors(h15, h53, h31->inv, v3, h31->edge, f3);
        h14->set_neighbors(h45, h51, h14->inv, v1, h14->edge, f1);
        h42->set_neighbors(h25, h54, h42->inv, v4, h42->edge, f4);

        v5->halfedge = h51;

        // 四个顶点分配半边
        v1->halfedge = h15;
        v2->halfedge = h25;
        v3->halfedge = h35;
        v4->halfedge = h45;

        // 删除不需要的元素

        erase(f01);
        erase(f02);
        erase(e);
        erase(h_inv);
        erase(h);

        return v5;
    } else {
        return std::nullopt;
    }
}

optional<Vertex*> HalfedgeMesh::collapse_edge(Edge* e)
{
    if (!e->on_boundary()) {
        // 获取半边
        Halfedge* h     = e->halfedge;
        Halfedge* h_inv = h->inv;

        // 获取顶点
        Vertex* v1 = h->from;
        Vertex* v2 = h_inv->from;
        Vertex* v3 = h->prev->from;
        Vertex* v4 = h_inv->prev->from;

        // 安全检查

        if (v1->degree() <= 3 || v2->degree() <= 3) {
            return std::nullopt;
        }

        //
        v1->pos = (v1->pos + v2->pos) / 2;

        //
        Halfedge* temp_h = h->inv;
        while (temp_h->prev->inv != h->inv) {
            temp_h       = temp_h->prev->inv;
            temp_h->from = v1;
        }

        h->prev->inv->set_neighbors(h->prev->inv->next, h->prev->inv->prev, h->next->inv, v1,
                                    h->prev->inv->edge, h->prev->inv->face);
        h->next->inv->set_neighbors(h->next->inv->next, h->next->inv->prev, h->prev->inv, v3,
                                    h->prev->inv->edge, h->next->inv->face);
        h->prev->inv->edge->halfedge = h->prev->inv;

        h->inv->next->inv->set_neighbors(h->inv->next->inv->next, h->inv->next->inv->prev,
                                         h->inv->prev->inv, v4, h->inv->next->inv->edge,
                                         h->inv->next->inv->face);
        h->inv->prev->inv->set_neighbors(h->inv->prev->inv->next, h->inv->prev->inv->prev,
                                         h->inv->next->inv, v1, h->inv->next->inv->edge,
                                         h->inv->prev->inv->face);
        h->inv->next->inv->edge->halfedge = h->inv->next->inv;

        // 顶点的对应半边
        v1->halfedge = h->prev->inv;
        v3->halfedge = h->next->inv;
        v4->halfedge = h->inv->next->inv;
        // 删除多余元素

        erase(h->inv->prev->edge);
        erase(h->inv->prev->face);
        erase(h->inv->prev);
        erase(h->inv->next->edge);
        erase(h->inv->next);
        erase(h->prev->edge);
        erase(h->prev->face);
        erase(h->prev);
        erase(h->next->edge);
        erase(h->next);
        erase(v2);
        erase(h->inv);
        erase(h->edge);
        erase(h);
        return v1;
    } else if (e->on_boundary()) {
        Halfedge* h;
        if (e->halfedge->is_boundary()) {
            h = e->halfedge->inv;
        } else {
            h = e->halfedge;
        }
        Vertex* v1 = h->from;
        Vertex* v2 = h->next->from;
        Vertex* v3 = h->prev->from;

        // 移动v1
        v1->pos = (v1->pos + v2->pos) / 2;

        // 遍历v2的所有半边
        Halfedge* temp_h = h->next;
        while (true) {
            temp_h->from = v1;
            if (temp_h->edge->on_boundary()) {
                break;
            }
            temp_h = temp_h->inv->next;
        }
        h->prev->inv->set_neighbors(h->prev->inv->next, h->prev->inv->prev, h->next->inv, v1,
                                    h->prev->inv->edge, h->prev->inv->face);
        h->next->inv->set_neighbors(h->next->inv->next, h->next->inv->prev, h->prev->inv, v3,
                                    h->prev->inv->edge, h->next->inv->face);
        h->prev->inv->edge->halfedge = h->prev->inv;

        // 绑定顶点半边

        v1->halfedge = h->prev->inv;
        v3->halfedge = h->next->inv;

        // 删除多余元素
        erase(h->face);
        erase(h->next->edge);
        erase(h->next);
        erase(h->prev);
        erase(h->edge);
        erase(h->inv);
        erase(v2);
        erase(h);
        return v1;
    } else {
        return std::nullopt;
    }
=======
optional<Edge*> HalfedgeMesh::flip_edge(Edge* e)
{
    // 非边界边
    if (!e->on_boundary()) {
        // 要用到的半边
        Halfedge* h     = e->halfedge;
        Halfedge* h_inv = h->inv;
        Halfedge* h_2_3 = h->next;
        Halfedge* h_3_1 = h_2_3->next;
        Halfedge* h_1_4 = h_inv->next;
        Halfedge* h_4_2 = h_1_4->next;
        // v3 and v4 are vertices opposite the edge
        Vertex* v1 = h->from;
        Vertex* v2 = h_inv->from;
        Vertex* v3 = h_3_1->from;
        Vertex* v4 = h_4_2->from;
        // 要用到的面片
        Face* f1 = h->face;
        Face* f2 = h_inv->face;
        // 重新连接各基本元素
        // 设置h
        h->next = h_3_1;
        h->prev = h_1_4;
        h->from = v4;
        // 设置h->inv
        h_inv->next = h_4_2;
        h_inv->prev = h_2_3;
        h_inv->from = v3;
        // 设置两个面绑定的半边
        f1->halfedge = h;
        f2->halfedge = h_inv;
        // 设置两个顶点绑定的半边
        v1->halfedge = h_1_4;
        v2->halfedge = h_2_3;
        // 更新外边
        h_2_3->set_neighbors(h_inv, h_4_2, h_2_3->inv, h_2_3->from, h_2_3->edge, f2);
        h_4_2->set_neighbors(h_2_3, h_inv, h_4_2->inv, h_4_2->from, h_4_2->edge, f2);
        h_3_1->set_neighbors(h_1_4, h, h_3_1->inv, h_3_1->from, h_3_1->edge, f1);
        h_1_4->set_neighbors(h, h_3_1, h_1_4->inv, h_1_4->from, h_1_4->edge, f1);

        return e;
    }

    else {
        return std::nullopt;
    }
}

optional<Vertex*> HalfedgeMesh::split_edge(Edge* e)
{
    if (!e->on_boundary()) {
        // 获取4个顶点
        Vertex* v1 = e->halfedge->from;
        Vertex* v2 = e->halfedge->inv->from;
        Vertex* v3 = e->halfedge->prev->from;
        Vertex* v4 = e->halfedge->inv->prev->from;

        // 获取边
        Halfedge* h     = e->halfedge;
        Halfedge* h_inv = e->halfedge->inv;
        Halfedge* h23   = h->next;
        Halfedge* h31   = h->prev;
        Halfedge* h42   = h_inv->prev;
        Halfedge* h14   = h_inv->next;

        // 获取面
        Face* f01 = h23->face;
        Face* f02 = h42->face;

        // 创建一个新的顶点，作为分割点
        Vertex* v5 = new_vertex();
        v5->pos    = (v2->pos + v1->pos) / 2;

        // 以边上四边创建4个面
        Face* f1     = new_face();
        f1->halfedge = h14;
        Face* f2     = new_face();
        f2->halfedge = h23;
        Face* f3     = new_face();
        f3->halfedge = h31;
        Face* f4     = new_face();
        f4->halfedge = h42;

        // 创建半边
        Halfedge* h15 = new_halfedge();
        Halfedge* h51 = new_halfedge();
        Halfedge* h25 = new_halfedge();
        Halfedge* h52 = new_halfedge();
        Halfedge* h35 = new_halfedge();
        Halfedge* h53 = new_halfedge();
        Halfedge* h45 = new_halfedge();
        Halfedge* h54 = new_halfedge();

        // 创建边
        Edge* e1     = new_edge();
        e1->halfedge = h15;
        Edge* e2     = new_edge();
        e2->halfedge = h25;
        Edge* e3     = new_edge();
        e3->halfedge = h35;
        Edge* e4     = new_edge();
        e4->halfedge = h45;

        // 设置半边
        h15->set_neighbors(h53, h31, h51, v1, e1, f3);
        h51->set_neighbors(h14, h45, h15, v5, e1, f1);
        h25->set_neighbors(h54, h42, h52, v2, e2, f4);
        h52->set_neighbors(h23, h35, h25, v5, e2, f2);
        h35->set_neighbors(h52, h23, h53, v3, e3, f2);
        h53->set_neighbors(h31, h15, h35, v5, e3, f3);
        h45->set_neighbors(h51, h14, h54, v4, e4, f1);
        h54->set_neighbors(h42, h25, h45, v5, e4, f4);

        // 更新周围4边
        h23->set_neighbors(h35, h52, h23->inv, v2, h23->edge, f2);
        h31->set_neighbors(h15, h53, h31->inv, v3, h31->edge, f3);
        h14->set_neighbors(h45, h51, h14->inv, v1, h14->edge, f1);
        h42->set_neighbors(h25, h54, h42->inv, v4, h42->edge, f4);

        v5->halfedge = h51;

        // 四个顶点分配半边
        v1->halfedge = h15;
        v2->halfedge = h25;
        v3->halfedge = h35;
        v4->halfedge = h45;

        // 删除不需要的元素

        erase(f01);
        erase(f02);
        erase(e);
        erase(h_inv);
        erase(h);

        return v5;
    } else {
        return std::nullopt;
    }
}

optional<Vertex*> HalfedgeMesh::collapse_edge(Edge* e)
{
    if (!e->on_boundary()) {
        // 获取半边
        Halfedge* h     = e->halfedge;
        Halfedge* h_inv = h->inv;

        // 获取顶点
        Vertex* v1 = h->from;
        Vertex* v2 = h_inv->from;
        Vertex* v3 = h->prev->from;
        Vertex* v4 = h_inv->prev->from;

        // 安全检查

        if (v1->degree() <= 3 || v2->degree() <= 3) {
            return std::nullopt;
        }

        //
        v1->pos = (v1->pos + v2->pos) / 2;

        //
        Halfedge* temp_h = h->inv;
        while (temp_h->prev->inv != h->inv) {
            temp_h       = temp_h->prev->inv;
            temp_h->from = v1;
        }

        h->prev->inv->set_neighbors(h->prev->inv->next, h->prev->inv->prev, h->next->inv, v1,
                                    h->prev->inv->edge, h->prev->inv->face);
        h->next->inv->set_neighbors(h->next->inv->next, h->next->inv->prev, h->prev->inv, v3,
                                    h->prev->inv->edge, h->next->inv->face);
        h->prev->inv->edge->halfedge = h->prev->inv;

        h->inv->next->inv->set_neighbors(h->inv->next->inv->next, h->inv->next->inv->prev,
                                         h->inv->prev->inv, v4, h->inv->next->inv->edge,
                                         h->inv->next->inv->face);
        h->inv->prev->inv->set_neighbors(h->inv->prev->inv->next, h->inv->prev->inv->prev,
                                         h->inv->next->inv, v1, h->inv->next->inv->edge,
                                         h->inv->prev->inv->face);
        h->inv->next->inv->edge->halfedge = h->inv->next->inv;

        // 顶点的对应半边
        v1->halfedge = h->prev->inv;
        v3->halfedge = h->next->inv;
        v4->halfedge = h->inv->next->inv;
        // 删除多余元素

        erase(h->inv->prev->edge);
        erase(h->inv->prev->face);
        erase(h->inv->prev);
        erase(h->inv->next->edge);
        erase(h->inv->next);
        erase(h->prev->edge);
        erase(h->prev->face);
        erase(h->prev);
        erase(h->next->edge);
        erase(h->next);
        erase(v2);
        erase(h->inv);
        erase(h->edge);
        erase(h);
        return v1;
    } else if (e->on_boundary()) {
        Halfedge* h;
        if (e->halfedge->is_boundary()) {
            h = e->halfedge->inv;
        } else {
            h = e->halfedge;
        }
        Vertex* v1 = h->from;
        Vertex* v2 = h->next->from;
        Vertex* v3 = h->prev->from;

        // 移动v1
        v1->pos = (v1->pos + v2->pos) / 2;

        // 遍历v2的所有半边
        Halfedge* temp_h = h->next;
        while (true) {
            temp_h->from = v1;
            if (temp_h->edge->on_boundary()) {
                break;
            }
            temp_h = temp_h->inv->next;
        }
        h->prev->inv->set_neighbors(h->prev->inv->next, h->prev->inv->prev, h->next->inv, v1,
                                    h->prev->inv->edge, h->prev->inv->face);
        h->next->inv->set_neighbors(h->next->inv->next, h->next->inv->prev, h->prev->inv, v3,
                                    h->prev->inv->edge, h->next->inv->face);
        h->prev->inv->edge->halfedge = h->prev->inv;

        // 绑定顶点半边

        v1->halfedge = h->prev->inv;
        v3->halfedge = h->next->inv;

        // 删除多余元素
        erase(h->face);
        erase(h->next->edge);
        erase(h->next);
        erase(h->prev);
        erase(h->edge);
        erase(h->inv);
        erase(v2);
        erase(h);
        return v1;
    } else {
        return std::nullopt;
    }
>>>>>>> master
}