#pragma once

#include "header.h"
#include "sectorcollection.h"
#include "directoryentry.h"
#include <queue>
#include <list>
#include <unordered_set>
#include "RBTree/rbtree.h"
#include "idirectoryentry.h"

#define FLAT_WRITE

namespace CFCPP
{
enum CFSConfiguration
{

    Default = 1,
    SectorRecycle = 2,
    EraseFreeSectors = 4,
    NoValidationException = 8,
    LeaveOpen = 16
};

enum CFSUpdateMode
{
    /// ReadOnly update mode prevents overwriting
    /// of the opened file.
    /// Data changes are allowed but they have to be
    /// persisted on a different file when required
    ReadOnly,

    /// Update mode allows subsequent data changing operations
    /// to be persisted directly on the opened file or stream
    /// method when required. Warning: this option may cause existing data loss if misused.
    Update
};

class CompoundFile
{
public:
    CompoundFile(const std::wstring &fileName, CFSUpdateMode updateMode, CFSConfiguration configParameters);
    CompoundFile(CFSVersion cfsVersion, CFSConfiguration configFlags);
    CompoundFile(const std::wstring &fileName);
    CompoundFile(Stream stream);
    CompoundFile();
    void OnSizeLimitReached();
    void Commit();
    void Commit(bool releaseMemory);
    inline bool HasSourceStream() {return sourceStream != nullptr;}

    void Close();

    std::shared_ptr<RedBlackTree::RBTree> CreateNewTree();
    std::shared_ptr<RedBlackTree::RBTree> GetChildrenTree(int sid);
    bool IsClosed()const;
protected:
    int GetSectorSize();

private:
    void CheckForLockSector();
    void LoadFile(std::wstring fileName);
    void SetFileName(std::wstring fileName);
    void LoadStream(Stream stream);
    void Load(Stream stream);

    void Save(std::wstring wFileName);
    void Save(Stream stream);

    SVector<Sector> GetFatSectorChain();
    SVector<Sector> GetDifatSectorChain();
    SVector<Sector> GetNormalSectorChain(int secID);
    SVector<Sector> GetMiniSectorChain(int secID);
    SVector<Sector> GetSectorChain(int secID, SectorType chainType);
    void EnsureUniqueSectorIndex(int nextSecID, std::unordered_set<int> &processedSectors);
    void CommitDirectory();
    void Close(bool closeStream);

    std::shared_ptr<IDirectoryEntry> RootEntry();
    SVector<IDirectoryEntry> FindDirectoryEntries(std::wstring entryName);

    std::shared_ptr<RedBlackTree::RBTree> DoLoadChildrenTrusted(std::shared_ptr<IDirectoryEntry> de);
    void DoLoadChildren(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de);
    void NullifyChildNodes(std::shared_ptr<IDirectoryEntry> de);
    void LoadSiblings(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de);
    void DoLoadSiblings(std::shared_ptr<RedBlackTree::RBTree> bst, std::shared_ptr<IDirectoryEntry> de);
    bool ValidateSibling(int sid);
    void LoadDirectories();

    inline CFSVersion getVersion() const {return (CFSVersion)header.majorVersion;}

public:
    CFSConfiguration configuration = Default;
    Header header;
    Stream sourceStream;

private:
    const int HEADER_DIFAT_ENTRIES_COUNT = 109;
    int DIFAT_SECTOR_FAT_ENTRIES_COUNT = 127;
    int FAT_SECTOR_ENTRIES_COUNT = 128;
    const int SIZE_OF_SID = 4;
    bool sectorRecycle = false;
    bool eraseFreeSectors = false;
    static constexpr int FLUSHING_QUEUE_SIZE = 6000;
    static constexpr int FLUSHING_BUFFER_MAX_SIZE = 1024 * 1024 * 16;
    SectorCollection sectors;
    std::fstream stream;
    std::string fileName;

    bool closeStream = true;
    bool _transactionLockAdded = false;
    int _lockSectorId = -1;
    bool _transactionLockAllocated = false;
    bool validationExceptionEnabled = true;
    bool _disposed;//false
    CFSUpdateMode updateMode;
    SVector<DirectoryEntry> directoryEntries;
    std::list<int> levelSIDs;

#if !defined(FLAT_WRITE)
        std::array<BYTE, FLUSHING_BUFFER_MAX_SIZE> buffer;
        std::queue<std::shared_ptr<Sector> > flushingQueue;
#endif
};
}
