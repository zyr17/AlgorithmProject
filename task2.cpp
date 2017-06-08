#include <bits/stdc++.h>
#define N 1111111

const std::string datafolder = "C:\\Users\\zyr17\\Documents\\Algorithm\\data\\";
const char ATCG[] = {'A', 'T', 'C', 'G'};
char s1[N];
int n, m, k;
int prev[N][4], next[N][4], dlow[N], dhigh[N], khighk[N];
unsigned char fa[N];
int l[11111111];

void savefa(unsigned char *fa, int length, int num){
	return;
	char buffer[100];
	sprintf(buffer, "%sfa%08d.dat", datafolder.c_str(), num);
	FILE *f = fopen(buffer, "wb");
	for (int i = 0; i < length; i += 2){
		unsigned char output = ((i != length - 1 ? fa[i + 1] : 0) << 4) + fa[i];
		fputc(output, f);
	}
	fclose(f);
}

void loadfa(unsigned char *fa, int length, int num){
	char buffer[100];
	sprintf(buffer, "%sfa%08d.dat", datafolder.c_str(), num);
	FILE *f = fopen(buffer, "rb");
	for (int i = 0; i < length; i += 2){
		int get = fgetc(f) & 0xff;
		fa[i] = get & 0xf;
		get >>= 4;
		if (i != length - 1)
			fa[i + 1] = get & 0xf;
	}
	fclose(f);
}

int main(){
	freopen("task3.in","r",stdin);
	//freopen("task1.out","w",stdout);
	scanf("%s%d", s1, &m);
	n = strlen(s1);
	std::map<std::string, std::vector<int>> prevmap, nextmap;
	std::vector<std::string> s2;
	for (int i = 0; i < m; i ++ ){
		char buffer[100];
		scanf("%s", buffer);
		k = strlen(buffer);
		s2.push_back(buffer);
		for (int j = 0; j < 4; j ++ )
			prev[i][j] = next[i][j] = - 1;
		std::string next = buffer + 1;
		buffer[k - 1] = 0;
		std::string prev = buffer;
		prevmap[prev].push_back(i);
		nextmap[next].push_back(i);
	}
	for (auto &oneprev : prevmap){
		auto &prevlist = oneprev.second;
		if (prevlist.size() == 0) continue;
		auto &nextlist = nextmap[oneprev.first];
		int temp[4] = {-1, -1, -1, -1};
		for (auto i : prevlist)
			for (int j = 0; j < 4; j ++ )
				if (s2[i][k - 1] == ATCG[j])
					temp[j] = i;
		for (auto i : nextlist)
			for (int j = 0; j < 4; j ++ )
				next[i][j] = temp[j];
	}
	for (auto &onenext : nextmap){
		auto &nextlist = onenext.second;
		if (nextlist.size() == 0) continue;
		auto &prevlist = prevmap[onenext.first];
		int temp[4] = {-1, -1, -1, -1};
		for (auto i : nextlist)
			for (int j = 0; j < 4; j ++ )
				if (s2[i][0] == ATCG[j])
					temp[j] = i;
		for (auto i : prevlist)
			for (int j = 0; j < 4; j ++ )
				prev[i][j] = temp[j];
	}
	/*for (int i = 0; i < m; i ++ ){
		for (int j = 0; j < 4; j ++ )
			printf("%d ", prev[i][j]);
		for (int j = 0; j < 4; j ++ )
			printf("%d ", next[i][j]);
			printf("\n");
	}*/
	std::vector<int> ll, done, startpoints;
	done.resize(m);
	for (int i = 0; i < m; i ++ )
		if (prev[i][0] + prev[i][1] + prev[i][2] + prev[i][3] == - 4)
			startpoints.push_back(i);
	ll = startpoints;
	for (unsigned t = 0; t < ll.size(); t ++ ){
		int i = ll[t];
		done[i] = 1;
		for (int j = 0; j < 4; j ++ )
			if (next[i][j] != - 1 && !done[next[i][j]]){
				ll.push_back(next[i][j]);
				done[next[i][j]] = 1;
			}
	}
	for (; ; ){
		int flag = - 1;
		for (int i = 0; i < m; i ++ )
			if (!done[i]) flag = i;
		if (flag == - 1) break;
		ll.clear();
		startpoints.push_back(flag);
		ll.push_back(flag);
		for (unsigned t = 0; t < ll.size(); t ++ ){
			int i = ll[t];
			done[i] = 1;
			for (int j = 0; j < 4; j ++ )
				if (next[i][j] != - 1 && !done[next[i][j]])
					ll.push_back(next[i][j]);
		}
	}
	std::vector<std::vector<int>> klow, khigh;
	klow.resize(m);
	khigh.resize(m);
	memset(dhigh, 63, sizeof dhigh);
	for (auto &i : khigh){
		i.resize(k + 1);
		for (int j = 0; j <= k; j ++ )
			i[j] = j;
	}
	/* fa format:
	 *
	 * XXXX | 1 | 22 | 3
	 * part 1: 0 = from d; 1 = from k or from selflow
	 * part 2: if from d, which father; other: no use
	 * part 3: if from d, low or high; if not from d: 0 = from k; 1 = from selflow
	 * 
	 * output:
	 * compress two bytes to one byte. if not enough, add zeros.
	 * fa[0]: XXXX1111; fa[1]: XXXX2222; fa[0] & fa[1]: 22221111
	 * fa[a]: XXXXaaaa; fa[a + 1]: null; fa[a] & null: 0000aaaa (XXXXaaaa is also allowed)
	 */
	for (int j = 0; j < n; j ++ ){
		//printf("%c ", s1[j]);
		if (j % 100 == 0 || j < 10000000) printf("doing %d\n", j);
		//int avaliablek = 0;
		for (int i = 0; i < m; i ++ ){
			if (khigh[i][k] == j - k){
				khighk[i] = khigh[i][k] = j - k + 1;
				//printf("x ", khigh[i][k]);
				continue;
			}
			//avaliablek ++ ;
			klow[i] = khigh[i];
			khigh[i][0] = j + 1;
			char nowchar = s1[j];
			for (int j = 1; j <= k; j ++ ){
				khigh[i][j] = khigh[i][j - 1] + 1;
				if (khigh[i][j] > klow[i][j] + 1)
					khigh[i][j] = klow[i][j] + 1;
				if (khigh[i][j] > klow[i][j - 1] + (nowchar != s2[i][j - 1]))
					khigh[i][j] = klow[i][j - 1] + (nowchar != s2[i][j - 1]);
			}
			khighk[i] = khigh[i][k];
			//printf("%d ", khigh[i][k]);
			//printf("| ");
		}
		//if (j % 100 == 0 || j < 1000) printf("avaliablek %d\n", avaliablek);
		//printf("\n");
		char nowchar = s1[j];
		memcpy(dlow, dhigh, sizeof dhigh);
		savefa(fa, m, j);
		memset(dhigh, 63, sizeof dhigh);
		for (unsigned i = 0; i < startpoints.size(); i ++ )
			l[i] = startpoints[i];
		unsigned lsize = startpoints.size();
		printf("%d\n", clock());
		for (unsigned T = 0; T < lsize; T ++ ){
		//for (; lsize; ){
			int t = l[T];
			int pastvalue = dhigh[t];
			for (int q = 0; q < 4; q ++ )
				if (prev[t][q] != - 1){
					int fanum = prev[t][q];
					if (dhigh[t] > dhigh[fanum] + 1){
						dhigh[t] = dhigh[fanum] + 1;
						fa[t] = q * 2 + 1;
					}
					if (dhigh[t] > dlow[fanum] + (nowchar != s2[t][k - 1])){
						dhigh[t] = dlow[fanum] + (nowchar != s2[t][k - 1]);
						fa[t] = q * 2;
					}
				}
			if (dhigh[t] > dlow[t] + 1){
				dhigh[t] = dlow[t] + 1;
				fa[t] = 8 + 1;
			}
			if (dhigh[t] > khighk[t]){
				dhigh[t] = khighk[t];
				fa[t] = 8;
			}
			if (pastvalue > dhigh[t]){
				for (int q = 0; q < 4; q ++ )
					if (next[t][q] != - 1)
						l[lsize ++ ] = next[t][q];
			}
		}
		printf("%d\n", clock());
		/*for (auto i : dhigh)
			printf("%d ", i);
		printf("\n");*/
		printf("l length %d\n", lsize);
	}
	int res = 999999999;
	for (auto i : dhigh)
		if (res > i) res = i;
	printf("%d", res);
	/*
	std::vector<std::string> ans;
	for (; fa[n][m]; ){
		int t = fa[n][m];
		char buf[100];
		if (t == 3){
			if (s1[n] != s2[m])
				sprintf(buf, "SUB %d %c\n", n - 1, s2[m]);
			else buf[0] = 0;
		}
		else if (t == 2){
			sprintf(buf, "DEL %d\n", n - 1);
		}
		else if (t == 1){
			sprintf(buf, "INS %d %c\n", n, s2[m]);
		}
		if (buf[0]) ans.push_back(buf);
		n += p[t][0];
		m += p[t][1];
	}
	for (int i = ans.size() - 1; i >= 0; i -- )
		printf("%s", ans[i].c_str());*/
}
