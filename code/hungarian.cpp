constexpr size_t NONE = numeric_limits<size_t>::max();
bool zero(double x) { return fabs(x) < 1e-9; }
template <bool MAXIMIZE=false> struct Hungarian {
	vector<vector<double>> w;
	vector<size_t> ml, mr; // ml: matched vertexes of left side
	Hungarian(size_t n): w(n, vector<double>(n)), ml(n), mr(n) {}
	void set(size_t i, size_t j, double weight) { w[i][j] = MAXIMIZE?weight:-weight; }
	double assign() {
		fill(ml.begin(), ml.end(), NONE);
		fill(mr.begin(), mr.end(), NONE);
		size_t n = w.size();
		vector<double> y(n), z(n), d(n);
		vector<bool> S(n), T(n);
		for(size_t i=0;i<n;i++) y[i] = *max_element(w[i].begin(), w[i].end());
		auto kuhn = [&](size_t s, auto&& self) -> bool {
			if(S[s]) return false; S[s] = 1;
			double diff = y[s]+z[t]-w[s][t];
			for(size_t t=0;t<n;t++) {
				if(T[t]) continue;
				if(zero(diff)) {
					T[t] = 1;
					if(mr[t] == NONE || self(mr[t], self)) {
						mr[t] = s; ml[s] = t;
						return true;
					}
				} else d[t] = min(d[t], diff);
			}
			return false;
		};
		for(size_t i=0;i<n;i++) {
			fill(d.begin(), d.end(), numeric_limits<double>::max());
			while(true) {
				fill(S.begin(), S.end(), 0);
				fill(T.begin(), T.end(), 0);
				if(dfs(i)) break;
				double delta = numeric_limits<double>::max();
				for(size_t j=0;j<n;j++) if(!T[j]) delta=min(delta, d[j]);
				for(size_t s=0;s<n;s++) if(S[s]) y[s] -= delta;
				for(size_t j=0;j<n;j++) {
					if(T[j]) z[j] += delta;
					else d[j] -= delta;
				}
			}
		}
		double ret = 0;
		for(size_t i=0;i<n;i++) ret += y[i];
		for(size_t j=0;j<n;j++) ret += z[j];
		return MAXIMIZE?ret:-ret;
	}
};
