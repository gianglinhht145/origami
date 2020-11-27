# origami
A crew of 4 people (well 4 threads) will make and store intricate origami pieces.

- One thread will cut paper (run paperCutter()) and put it on the paperTable.
- Two threads will get the paper from the paperTable, fold the paper (run paperFolder()), and put the finished pieces on the finishedTable.
- One thread will get origami pieces from the finishedTable and put them away (run stocker()).

Both tables have a fixed size. A thread cannot place paper or an origami piece on a table if it is full.
