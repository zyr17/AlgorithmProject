#include <bits/stdc++.h>
#define N 1111111

struct dsafnieg{
	int prev[4], next[4], dlow, dhigh, khighk, fa, s2k;
}str[N];

const std::string datafolder = "C:\\Users\\zyr17\\Documents\\Algorithm\\data\\";
const char ATCG[] = {'A', 'T', 'C', 'G'};
char s1[N], s2k[N];
int n, m, k;
int prev[N][4], next[N][4], khighk[N];
//int dlow[N], dhigh[N];
unsigned char fa[N];
int l[11111111];

void savefa(int length, int num){
	char buffer[100];
	sprintf(buffer, "%sfa%08d.dat", datafolder.c_str(), num);
	FILE *f = fopen(buffer, "wb");
	for (int i = 0; i < length; i += 2){
		unsigned char output = ((i != length - 1 ? str[i + 1].fa : 0) << 4) + str[i].fa;
		fputc(output, f);
	}
	fclose(f);
}

void loadfa(int length, int num){
	char buffer[100];
	sprintf(buffer, "%sfa%08d.dat", datafolder.c_str(), num);
	FILE *f = fopen(buffer, "rb");
	for (int i = 0; i < length; i += 2){
		int get = fgetc(f) & 0xff;
		str[i].fa = get & 0xf;
		get >>= 4;
		if (i != length - 1)
			str[i + 1].fa = get & 0xf;
	}
	fclose(f);
}

std::vector<std::string> smallstep(std::string s1, std::string s2){
	//std::cout << s1 << ' ' << s2 << std::endl;
	n = s1.size();
	m = s2.size();
	s1 = " " + s1;
	s2 = " " + s2;
	std::vector<std::vector<int>> d, fa;
	d.resize(1000);
	fa.resize(1000);
	for (auto &i : d){
		i.resize(1000);
		for (auto &j : i)
			j = 999999999;
	}
	for (auto &i : fa)
		i.resize(1000);
	for (int i = 0; i <= n; i ++ ){
		d[i][0] = i;
		fa[i][0] = 2;
	}
	for (int j = 0; j <= m; j ++ ){
		d[0][j] = j;
		fa[0][j] = 1;
	}
	fa[0][0] = 0;
	for (int i = 1; i <= n; i ++ )
		for (int j = 1; j <= m; j ++ ){
			if (d[i][j] > d[i - 1][j - 1] + (s1[i] != s2[j])){
				d[i][j] = d[i - 1][j - 1] + (s1[i] != s2[j]);
				fa[i][j] = 3;
			}
			if (d[i][j] > d[i - 1][j] + 1){
				d[i][j] = d[i - 1][j] + 1;
				fa[i][j] = 2;
			}
			if (d[i][j] > d[i][j - 1] + 1){
				d[i][j] = d[i][j - 1] + 1;
				fa[i][j] = 1;
			}
		}
	//printf("%d\n", d[n][m]);
	int p[4][2] = {{0,0},{0,-1},{-1,0},{-1,-1}};
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
	//for (int i = ans.size() - 1; i >= 0; i -- )
	//	printf("%s", ans[i].c_str());
	return ans;
}

int main(){
	freopen("task2_modified.in","r",stdin);
	scanf("%s%d", s1, &m);
	n = strlen(s1);
	std::map<std::string, std::vector<int>> prevmap, nextmap;
	std::vector<std::string> s2;
	for (int i = 0; i < m; i ++ ){
		char buffer[100];
		scanf("%s", buffer);
		k = strlen(buffer);
		s2.push_back(buffer);
		str[i].s2k = s2k[i] = buffer[k - 1];
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
	
	for (int i = 0; i < m; i ++ )
		for (int j = 0; j < 4; j ++ ){
			str[i].prev[j] = prev[i][j];
			str[i].next[j] = next[i][j];
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
	//memset(dhigh, 63, sizeof dhigh);
	for (int i = 0; i < m; i ++ )
		str[i].dhigh = 999999999;
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
	 * part 3: if from d, 0 = low or 1 = high; if not from d: 1 = from k; 0 = from selflow
	 * 
	 * output:
	 * compress two bytes to one byte. if not enough, add zeros.
	 * fa[0]: XXXX1111; fa[1]: XXXX2222; fa[0] & fa[1]: 22221111
	 * fa[a]: XXXXaaaa; fa[a + 1]: null; fa[a] & null: 0000aaaa (XXXXaaaa is also allowed)
	 */
	for (int j = 0; j < n; j ++ ){
		//printf("%c ", s1[j]);
		if (j % 100 == 0 || j < 100000) printf("doing %d\n", j);
		//int avaliablek = 0;
		for (int i = 0; i < m; i ++ ){
			if (khigh[i][k] == j - k){
				str[i].khighk = khighk[i] = khigh[i][k] = j - k + 1;
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
			str[i].khighk = khighk[i] = khigh[i][k];
			//printf("%d ", khigh[i][k]);
			//printf("| ");
		}
		//if (j % 100 == 0 || j < 1000) printf("avaliablek %d\n", avaliablek);
		//printf("\n");
		char nowchar = s1[j];
		//memcpy(dlow, dhigh, sizeof dhigh);
		for (int i = 0; i < m; i ++ )
			str[i].dlow = str[i].dhigh;
		savefa(m, j);
		//memset(dhigh, 63, sizeof dhigh);
		for (int i = 0; i < m; i ++ )
			str[i].dhigh = 999999999;
		for (unsigned i = 0; i < startpoints.size(); i ++ )
			l[i] = startpoints[i];
		unsigned lsize = startpoints.size();
		//printf("%d\n", clock());
		for (unsigned T = 0; T < lsize; T ++ ){
		//for (; lsize; ){
			int t = l[T];
			int pastvalue = str[t].dhigh;
			for (int q = 0; q < 4; q ++ )
				if (str[t].prev[q] != - 1){
					int fanum = str[t].prev[q];
					if (str[t].dhigh > str[fanum].dhigh + 1){
						str[t].dhigh = str[fanum].dhigh + 1;
						str[t].fa = q * 2 + 1;
					}
					if (str[t].dhigh > str[fanum].dlow + (nowchar != str[t].s2k)){
						str[t].dhigh = str[fanum].dlow + (nowchar != str[t].s2k);
						str[t].fa = q * 2;
					}
				}
			if (str[t].dhigh > str[t].dlow + 1){
				str[t].dhigh = str[t].dlow + 1;
				str[t].fa = 8;
			}
			if (str[t].dhigh > str[t].khighk){
				str[t].dhigh = str[t].khighk;
				str[t].fa = 8 + 1;
			}
			if (pastvalue > str[t].dhigh){
				for (int q = 0; q < 4; q ++ )
					if (str[t].next[q] != - 1)
						l[lsize ++ ] = str[t].next[q];
			}
		}
		//printf("%d\n", clock());
		/*for (auto i : dhigh)
			printf("%d ", i);
		printf("\n");*/
		//printf("l length %d %d\n", lsize, startpoints.size());
	}
	int res = 999999999;
	int nown = n - 1, nowm = 0;
	for (int i = 0; i < m; i ++ )
		if (res > str[i].dhigh){
			res = str[i].dhigh;
			nowm = i;
		}
	
	std::vector<std::string> ansstep;
	std::string ansstr;
	for (; ; ){
		int t = str[nowm].fa;
		if (ansstr.size() < 100) printf("%c %d %d %d\n", str[nowm].s2k, nown, nowm, t);
		char buf[100];
		if (t & 8){
			//from k or from selflow
			if (t & 1){
				//from k
				for (int i = k - 1; i >= 0; i -- )
					ansstr += s2[nowm][i];
				s1[nown + 1] = 0;
				auto kstep = smallstep(s1, s2[nowm]);
				for (auto &i : kstep)
					ansstep.push_back(i);
				break;
			}
			else{
				//from selflow
				sprintf(buf, "DEL %d\n", nown);
				ansstep.push_back(buf);
				loadfa(m, nown -- );
			}
		}
		else{
			//from prev
			int from = str[nowm].prev[(t & 7) / 2];
			if (t & 1){
				//from high
				sprintf(buf, "INS %d %c\n", nown + 1, str[nowm].s2k);
				ansstr.push_back(str[nowm].s2k);
				nowm = from;
				ansstep.push_back(buf);
			}
			else{
				//from low
				if (str[nowm].s2k != s1[nown]){
					sprintf(buf, "SUB %d %c\n", nown, str[nowm].s2k);
					ansstep.push_back(buf);
				}
				ansstr.push_back(str[nowm].s2k);
				nowm = from;
				loadfa(m, nown -- );
			}
		}
	}
	freopen("task3.out","w",stdout);
	for (int i = ansstr.size() - 1; i >= 0; i -- )
		printf("%c", ansstr[i]);
	printf("\n");
	printf("%d\n", res);
	for (int i = ansstep.size() - 1; i >= 0; i -- )
		printf("%s", ansstep[i].c_str());
}
