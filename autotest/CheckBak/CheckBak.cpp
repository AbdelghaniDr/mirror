#include <Core/Core.h>

using namespace Upp;

void GetRepoInfo(String repo, Date& d, int& rev)
{
	String s = Sys("svn info svn://localhost/upp");
	LOG("SVN info:");
	LOG(s);

	String key = "Revision: ";
	int q = s.Find(key);
	ASSERT(q >= 0);
	rev = atoi(s.Mid(q + key.GetCount()));
	ASSERT(rev > 0);

	key = "Last Changed Date: ";
	q = s.Find(key);
	ASSERT(q >= 0);
	s = s.Mid(q + key.GetCount());
	ASSERT(s.GetCount() > 18);

	// 2014-10-30 01:01:56
	// 0123456789012345678
	d.year = atoi(s);
	d.month = atoi(~s + 5);
	d.day = atoi(~s + 8);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	Date d, d1;
	int  rev, rev1;
	
	GetRepoInfo("svn://localhost/upp", d, rev);

	GetRepoInfo("svn://www.ultimatepp.org/upp", d1, rev1);

	LOG("Main repository last date: " << d1);
	LOG("Main repository revision: " << rev1);

	LOG("Mirror repository last date: " << d);
	LOG("Mirror repository revision: " << rev);

	ASSERT(d == d1 && rev == rev1);
	
	for(int pass = 0; pass < 3; pass++) {
		String dir = decode(pass, 0, "/mnt/nas1/bak", 1, "/mnt/nas/bak", "/bak");
		FindFile ff(dir + "/*");
		Time tm = Time::Low();
		int64 len = 0;
		String fn;
		
		while(ff) {
			if(ff.IsFile()) {
				Time tm1 = Time(ff.GetLastWriteTime());
				if(tm1 > tm) {
					tm = tm1;
					len = ff.GetLength();
					fn = ff.GetPath();
				}
			}
			ff.Next();
		}
		
		len = len >> 20;
		LOG(fn << ", last time: " << tm << ", size " << len << " MB");
	
		ASSERT(tm >= GetSysDate() - 1);
		ASSERT(len > 2000 && len < 2800);

		String h = Sys("tar -tvf " + fn);
		int q = h.Find("root/website.bak/root/bak/forum.fud");
		ASSERT(q >= 17);
		String s = h.Mid(q - 17, 17);
		Date d;
		// 2014-10-30 01:01:56
		// 0123456789012345678
		d.year = atoi(s);
		d.month = atoi(~s + 5);
		d.day = atoi(~s + 8);
		LOG("forum.fud last date: " << d);
		ASSERT(d >= GetSysDate() - 1);
	}

	
	String s = HttpRequest("http://www.ultimatepp.org/df.info").Execute();
	LOG(s);
	int q = s.Find("%");
	ASSERT(q >= 0);
	q = s.Find("%", q + 1);
	ASSERT(q >= 4);
	int n = atoi(~s + q - 3);
	LOG("Filesystem uses " << n << "%");
	ASSERT(n > 10 && n < 85);
	
	LOG("------------------- OK");
}
