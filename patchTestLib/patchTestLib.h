#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// アイテムを左右の2列で表示する
// 左がインベントリ、右が倉庫
// クリックしたら右か左に移動する。確認は行わない。
namespace NSPatchTestLib
{
class ISprite
{
public:
    virtual void DrawImage(const int x, const int y, const int transparency = 255) = 0;
    virtual void Load(const std::wstring& filepath) = 0;
    virtual ~ISprite() {};
};

class IFont
{
public:
    virtual void DrawText_(const std::wstring& msg,
                           const int x,
                           const int y,
                           const int transparent = 255) = 0;
    virtual void Init(const bool bEnglish) = 0;
    virtual ~IFont() {};
};

class ISoundEffect
{
public:
    virtual void PlayMove() = 0;
    virtual void PlayClick() = 0;
    virtual void PlayBack() = 0;
    virtual void Init() = 0;
    virtual ~ISoundEffect() {};
};

std::wstring CreateDateTimeStr(const int y, const int M, const int d,
                              const int h, const int m, const int s);

class TestItem
{
public:
    void SetId(const std::wstring& arg);
    std::wstring GetId() const;

    void SetSubId(const int arg);
    int GetSubId() const;

    void SetName(const std::wstring& arg);
    std::wstring GetName() const;

private:

    std::wstring m_id;
    int m_idSub = 0;
    std::wstring m_name;
};

class QueuedTestItem
{
public:

    void SetName(const std::wstring& arg);
    std::wstring GetName() const;

    void SetDateReq(const int year,
                    const int month,
                    const int day,
                    const int hour,
                    const int minute,
                    const int second);

    void GetDateReq(int* year,
                    int* month,
                    int* day,
                    int* hour,
                    int* minute,
                    int* second);

    std::wstring GetDateReqStr();

    void SetDateStart(const int year,
                      const int month,
                      const int day,
                      const int hour,
                      const int minute,
                      const int second);

    void GetDateStart(int* year,
                      int* month,
                      int* day,
                      int* hour,
                      int* minute,
                      int* second);

    std::wstring GetDateStartStr();

    void SetDateEnd(const int year,
                    const int month,
                    const int day,
                    const int hour,
                    const int minute,
                    const int second);

    void GetDateEnd(int* year,
                    int* month,
                    int* day,
                    int* hour,
                    int* minute,
                    int* second);

    std::wstring GetDateEndStr();

    void SetResult(const std::wstring& arg);
    std::wstring GetResult() const;

private:

    std::wstring m_name;

    int m_reqYear = 0;
    int m_reqMonth = 0;
    int m_reqDay = 0;
    int m_reqHour = 0;
    int m_reqMinute = 0;
    int m_reqSecond = 0;

    int m_startYear = 0;
    int m_startMonth = 0;
    int m_startDay = 0;
    int m_startHour = 0;
    int m_startMinute = 0;
    int m_startSecond = 0;

    int m_endYear = 0;
    int m_endMonth = 0;
    int m_endDay = 0;
    int m_endHour = 0;
    int m_endMinute = 0;
    int m_endSecond = 0;

    std::wstring m_result;

};

class PatchTestLib
{
public:

    void Init(IFont* font,
              ISoundEffect* SE,
              ISprite* sprCursor,
              ISprite* sprBackground,
              ISprite* sprVBar,
              const bool bEnglish);

    void Finalize();

    void AddTestItem(const TestItem& arg);
    void AddQueueItem(const QueuedTestItem& arg);

    void ClearAll();

    void MoveFromInventoryToQueue(const std::wstring& id,
                                  const int subid,
                                  const int year,
                                  const int month,
                                  const int day,
                                  const int hour,
                                  const int minute,
                                  const int second);

    // 一番古いもののindexが0
    void UpdateQueueItemStatus(const int index,
                               const int yearStart,
                               const int monthStart,
                               const int dayStart,
                               const int hourStart,
                               const int minuteStart,
                               const int secondStart,
                               const int yearEnd,
                               const int monthEnd,
                               const int dayEnd,
                               const int hourEnd,
                               const int minuteEnd,
                               const int secondEnd,
                               const std::wstring& result
                               );

    void UpdateDateTime(const int year,
                        const int month,
                        const int day,
                        const int hour,
                        const int minute,
                        const int second);

    std::wstring Up();
    std::wstring Down();
    std::wstring Right();
    std::wstring Left();
    std::wstring Into();
    std::wstring Back();
    std::wstring Next();
    std::wstring Previous();
    void CursorOn(const int x, const int y);
    std::wstring Click(const int x, const int y);
    void Draw();
    void UpdateCursorPos();

    template <typename T>
    static void EraseRemove(std::vector<T>& vec, const T& value)
    {
        vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
    }

    template <typename T, typename Predicate>
    static void EraseRemoveIf(std::vector<T>& vec, Predicate pred)
    {
        vec.erase(std::remove_if(vec.begin(), vec.end(), pred), vec.end());
    }
    
private:

    enum class eFocus
    {
        LEFT,
        RIGHT,
    };

    ISprite* m_sprCursor = nullptr;
    ISprite* m_sprBackground = nullptr;
    ISprite* m_sprVBar = nullptr;
    IFont* m_font = nullptr;
    ISoundEffect* m_SE = nullptr;
    eFocus m_eFocus = eFocus::LEFT;

    std::vector<TestItem> m_leftList;
    std::vector<QueuedTestItem> m_rightList;

    const int LEFT_PANEL_WIDTH = 250;
    const int RIGHT_PANEL_WIDTH = 500;
    const int PANEL_HEIGHT = 60;

    const int LEFT_PANEL_STARTX = 100;
    const int LEFT_PANEL_STARTY = 200;

    const int RIGHT_PANEL_STARTX = 400;
    const int RIGHT_PANEL_STARTY = 200;

    const int PANEL_ROW_MAX = 10;

    // スクロール可能であることを考慮する
    // 上から画面上で何番目にカーソルがあるか。
    int m_leftCursor = 0;

    // カーソルが選択している要素がm_outputListの何番目の要素か。
    int m_leftSelect = 0;

    // 何番目のアイテムが一番上に表示されているか
    // スクロール可能なので一番上に表示されるアイテムはスクロールすると変わる。
    int m_leftBegin = 0;

    // スクロール可能であることを考慮する
    // 上から画面上で何番目にカーソルがあるか。
    int m_rightCursor = 0;

    // カーソルが選択している要素がm_outputListの何番目の要素か。
    int m_rightSelect = 0;

    // 何番目のアイテムが一番上に表示されているか
    // スクロール可能なので一番上に表示されるアイテムはスクロールすると変わる。
    int m_rightBegin = 0;

    std::wstring m_CurrentDateTime;

    bool m_bEnglish = false;
};
}

