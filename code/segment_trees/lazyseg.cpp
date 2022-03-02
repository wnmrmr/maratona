struct LazySeg {
	struct Node {
		size_t lchild, rchild;
		int64_t pool;
		int64_t data;
		Node(): lchild(0), rchild(0), pool(0), data(0) {}
	};
	const size_t size;
	vector<Node> nodes;
	size_t new_node() {
		nodes.push_back(Node());
		return nodes.size()-1;
	}
	void refresh(Node& cur, size_t cl, size_t cr) {
		if(cur.pool) {
			cur.data += cur.pool*((int64_t)cr-cl+1);
			if(cl < cr) {
				Node& p1 = nodes[cur.lchild];
				Node& p2 = nodes[cur.rchild];
				p1.pool += cur.pool;
				p2.pool += cur.pool;
			}
			cur.pool = 0;
		}
	}
	template <typename T> LazySeg(size_t size, const T v[]): size(size) {
		nodes.reserve(2*size-1);
		new_node();
		build(nodes[0],0,size-1,v);
	}
	template <typename T> void build(Node& cur, size_t cl, size_t cr, const T v[]) {
		if(cl == cr) {
			cur.data = v[cl];
			return;
		}
		size_t mid = (cl+cr)/2;
		cur.lchild = new_node();
		Node& p1 = nodes[cur.lchild];
		cur.rchild = new_node();
		Node& p2 = nodes[cur.rchild];
		build(p1, cl, mid, v);
		build(p2, mid+1, cr, v);
		cur.data = p1.data + p2.data;
	}
	int64_t query(size_t l, size_t r, Node& cur, size_t cl, size_t cr) {
		refresh(cur, cl, cr);
		if(r < cl || cr < l) {
			return 0;
		}
		if(l <= cl && cr <= r) {
			return cur.data;
		}
		size_t mid = (cl+cr)/2;
		Node& p1 = nodes[cur.lchild];
		Node& p2 = nodes[cur.rchild];
		return query(l,r,p1,cl,mid) + query(l,r,p2,mid+1,cr);
	}
	void update(size_t l, size_t r, int64_t val, Node& cur, size_t cl, size_t cr) {
		refresh(cur, cl, cr);
		if(r < cl || cr < l) {
			return;
		}
		if(l <= cl && cr <= r) {
			cur.pool += val;
			refresh(cur, cl, cr);
			return;
		}
		size_t mid = (cl+cr)/2;
		Node& p1 = nodes[cur.lchild];
		Node& p2 = nodes[cur.rchild];
		update(l, r, val, p1, cl, mid);
		update(l, r, val, p2, mid+1, cr);
		cur.data = p1.data + p2.data;
	}
	int64_t query(size_t l, size_t r) {
		Node& cur = nodes[0];
		return query(l,r,cur,0,size-1);
	}
	void update(size_t l, size_t r, int64_t val) {
		Node& cur = nodes[0];
		update(l,r,val,cur,0,size-1);
	}
};
