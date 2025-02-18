#include "PatchTestLib.h"
#include <sstream>
#include <algorithm>

using namespace NSPatchTestLib;

static std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

void PatchTestLib::Init(IFont* font,
                        ISoundEffect* SE,
                        ISprite* sprCursor,
                        ISprite* sprBackground,
                        ISprite* sprVBar)
{
    m_font = font;
    m_SE = SE;
    m_sprCursor = sprCursor;
    m_sprBackground = sprBackground;
    m_sprVBar = sprVBar;
}

void NSPatchTestLib::PatchTestLib::Finalize()
{
    delete m_font;
    m_font = nullptr;

    delete m_SE;
    m_SE = nullptr;

    delete m_sprCursor;
    m_sprCursor = nullptr;

    delete m_sprBackground;
    m_sprBackground = nullptr;

    delete m_sprVBar;
    m_sprVBar = nullptr;
}

void NSPatchTestLib::PatchTestLib::AddTestItem(const TestItem& arg)
{
    m_leftList.push_back(arg);
}

void NSPatchTestLib::PatchTestLib::AddQueueItem(const QueuedTestItem& arg)
{
    m_rightList.push_back(arg);
}

void NSPatchTestLib::PatchTestLib::ClearAll()
{
    m_leftList.clear();
    m_rightList.clear();
}

void NSPatchTestLib::PatchTestLib::MoveFromInventoryToQueue(const int id, const int subid,
                                                            const int year,
                                                            const int month,
                                                            const int day,
                                                            const int hour,
                                                            const int minute,
                                                            const int second)
{
    auto it = std::find_if(m_leftList.begin(), m_leftList.end(),
                           [&](const TestItem& x)
                           {
                               return x.GetId() == id && x.GetSubId() == subid;
                           });

    if (it == m_leftList.end())
    {
        throw std::exception();
    }

    TestItem item = *it;
    m_leftList.erase(it);

    QueuedTestItem queItem;
    queItem.SetName(it->GetName());

    queItem.SetDateReq(year, month, day, hour, minute, second);
    m_rightList.push_back(queItem);
}

void NSPatchTestLib::PatchTestLib::UpdateQueueItemStatus(const int index,
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
                                                         const std::string& result)
{
    m_rightList.at(index).SetDateStart(yearStart, monthStart, dayStart,
                                       hourStart, minuteStart, secondStart);

    m_rightList.at(index).SetDateEnd(yearEnd, monthEnd, dayEnd,
                                     hourEnd, minuteEnd, secondEnd);

    m_rightList.at(index).SetResult(result);
}

void NSPatchTestLib::PatchTestLib::UpdateDateTime(const int year,
                                                  const int month,
                                                  const int day,
                                                  const int hour,
                                                  const int minute,
                                                  const int second)
{
    m_CurrentDateTime = CreateDateTimeStr(year, month, day, hour, minute, second);
}

std::string PatchTestLib::Up()
{
    if (m_eFocus == eFocus::LEFT)
    {
        auto previousSelect = m_leftSelect;
        if (m_leftSelect >= 1)
        {
            m_leftSelect--;
            m_SE->PlayMove();

            // カーソルが一番上にあるときに上ボタンを押されたら
            // カーソルはそのままでリストが下に移動する
            if (m_leftCursor != 0)
            {
                m_leftCursor--;
            }
            else if (m_leftCursor == 0)
            {
                if (previousSelect != 0)
                {
                    m_leftBegin--;
                }
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        auto previousSelect = m_rightSelect;
        if (m_rightSelect >= 1)
        {
            m_rightSelect--;
            m_SE->PlayMove();

            // カーソルが一番上にあるときに上ボタンを押されたら
            // カーソルはそのままでリストが下に移動する
            if (m_rightCursor != 0)
            {
                m_rightCursor--;
            }
            else if (m_rightCursor == 0)
            {
                if (previousSelect != 0)
                {
                    m_rightBegin--;
                }
            }
        }
    }
    return "";
}

std::string PatchTestLib::Down()
{
    if (m_eFocus == eFocus::LEFT)
    {
        auto previousSelect = m_leftSelect;
        if (m_leftSelect <= (int)m_leftList.size() - 2)
        {
            m_leftSelect++;
            m_SE->PlayMove();

            // カーソルが一番下にあるときに下ボタンを押されたら
            // カーソルはそのままでリストが上に移動する
            if (m_leftCursor != PANEL_ROW_MAX - 1)
            {
                m_leftCursor++;
            }
            else if (m_leftCursor == PANEL_ROW_MAX - 1)
            {
                if (previousSelect != (int)m_leftList.size() - 1)
                {
                    m_leftBegin++;
                }
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        auto previousSelect = m_rightSelect;
        if (m_rightSelect <= (int)m_rightList.size() - 2)
        {
            m_rightSelect++;
            m_SE->PlayMove();

            // カーソルが一番下にあるときに下ボタンを押されたら
            // カーソルはそのままでリストが上に移動する
            if (m_rightCursor != PANEL_ROW_MAX - 1)
            {
                m_rightCursor++;
            }
            else if (m_rightCursor == PANEL_ROW_MAX - 1)
            {
                if (previousSelect != (int)m_rightList.size() - 1)
                {
                    m_rightBegin++;
                }
            }
        }
    }
    return "";
}

std::string PatchTestLib::Right()
{
    if (m_eFocus == eFocus::LEFT)
    {
        m_eFocus = eFocus::RIGHT;
        m_SE->PlayMove();
    }
    return std::string();
}

std::string PatchTestLib::Left()
{
    if (m_eFocus == eFocus::RIGHT)
    {
        m_eFocus = eFocus::LEFT;
        m_SE->PlayMove();
    }
    return std::string();
}

std::string PatchTestLib::Into()
{
    std::string result;
    if (m_eFocus == eFocus::LEFT)
    {
        result = "left:";
        result += m_leftList.at(m_leftSelect).GetName();
        result += ":" + std::to_string(m_leftList.at(m_leftSelect).GetId());
        result += ":" + std::to_string(m_leftList.at(m_leftSelect).GetSubId());
        m_SE->PlayClick();
    }

    return result;
}

std::string PatchTestLib::Back()
{
    std::string result;
    result = "EXIT";
    m_SE->PlayBack();

    return result;
}

std::string NSPatchTestLib::PatchTestLib::Next()
{
    if (m_eFocus == eFocus::LEFT)
    {
        auto previousSelect = m_leftSelect;
        if (m_leftSelect <= (int)m_leftList.size() - 2)
        {
            m_leftSelect++;
            m_SE->PlayMove();
        }

        // カーソルが一番下にあるときに下ボタンを押されたら
        // カーソルはそのままでリストが上に移動する
        if (m_leftCursor != PANEL_ROW_MAX - 1)
        {
            m_leftCursor++;
        }
        else if (m_leftCursor == PANEL_ROW_MAX - 1)
        {
            if (previousSelect != (int)m_leftList.size() - 1)
            {
                m_leftBegin++;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        auto previousSelect = m_rightSelect;
        if (m_rightSelect <= (int)m_rightList.size() - 2)
        {
            m_rightSelect++;
            m_SE->PlayMove();
        }

        // カーソルが一番下にあるときに下ボタンを押されたら
        // カーソルはそのままでリストが上に移動する
        if (m_rightCursor != PANEL_ROW_MAX - 1)
        {
            m_rightCursor++;
        }
        else if (m_rightCursor == PANEL_ROW_MAX - 1)
        {
            if (previousSelect != (int)m_rightList.size() - 1)
            {
                m_rightBegin++;
            }
        }
    }
    return "";
}

std::string NSPatchTestLib::PatchTestLib::Previous()
{
    if (m_eFocus == eFocus::LEFT)
    {
        auto previousSelect = m_leftSelect;
        if (m_leftSelect >= 1)
        {
            m_leftSelect--;
            m_SE->PlayMove();
        }
        // カーソルが一番上にあるときに上ボタンを押されたら
        // カーソルはそのままでリストが下に移動する
        if (m_leftCursor != 0)
        {
            m_leftCursor--;
        }
        else if (m_leftCursor == 0)
        {
            if (previousSelect != 0)
            {
                m_leftBegin--;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        auto previousSelect = m_rightSelect;
        if (m_rightSelect >= 1)
        {
            m_rightSelect--;
            m_SE->PlayMove();
        }
        // カーソルが一番上にあるときに上ボタンを押されたら
        // カーソルはそのままでリストが下に移動する
        if (m_rightCursor != 0)
        {
            m_rightCursor--;
        }
        else if (m_rightCursor == 0)
        {
            if (previousSelect != 0)
            {
                m_rightBegin--;
            }
        }
    }
    return "";
}

void PatchTestLib::CursorOn(const int x, const int y)
{
    int previousCursor = m_leftCursor;
    int previousSelect = m_leftSelect;

    if (LEFT_PANEL_STARTX < x && x <= LEFT_PANEL_STARTX + LEFT_PANEL_WIDTH)
    {
        if (LEFT_PANEL_STARTY < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 1)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 0;
            m_leftSelect = 0 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 1 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 2)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 1;
            m_leftSelect = 1 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 2 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 3)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 2;
            m_leftSelect = 2 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 3 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 4)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 3;
            m_leftSelect = 3 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 4 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 5)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 4;
            m_leftSelect = 4 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 5 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 6)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 5;
            m_leftSelect = 5 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 6 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 7)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 6;
            m_leftSelect = 6 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 7 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 8)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 7;
            m_leftSelect = 7 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 8 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 9)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 8;
            m_leftSelect = 8 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 9 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 10)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 9;
            m_leftSelect = 9 + m_leftBegin;
        }
    }

    if ((size_t)m_leftSelect >= m_leftList.size())
    {
        m_leftSelect = previousSelect;
        m_leftCursor = previousCursor;
    }

    if (previousCursor != m_leftCursor)
    {
        m_SE->PlayMove();
    }

    int previousRightCursor = m_rightCursor;
    int previousRightSelect = m_rightSelect;

    if (RIGHT_PANEL_STARTX < x && x <= RIGHT_PANEL_STARTX + RIGHT_PANEL_WIDTH)
    {
        if (RIGHT_PANEL_STARTY < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 1)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 0;
            m_rightSelect = 0 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 1 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 2)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 1;
            m_rightSelect = 1 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 2 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 3)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 2;
            m_rightSelect = 2 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 3 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 4)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 3;
            m_rightSelect = 3 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 4 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 5)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 4;
            m_rightSelect = 4 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 5 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 6)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 5;
            m_rightSelect = 5 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 6 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 7)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 6;
            m_rightSelect = 6 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 7 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 8)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 7;
            m_rightSelect = 7 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 8 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 9)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 8;
            m_rightSelect = 8 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 9 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 10)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 9;
            m_rightSelect = 9 + m_rightBegin;
        }
    }

    if ((size_t)m_rightSelect >= m_rightList.size())
    {
        m_rightSelect = previousRightSelect;
        m_rightCursor = previousRightCursor;
    }

    if (previousRightCursor != m_rightCursor)
    {
        m_SE->PlayMove();
    }
}

std::string PatchTestLib::Click(const int x, const int y)
{
    std::string result;
    int previousCursor = m_leftCursor;
    int previousSelect = m_leftSelect;

    if (m_eFocus == eFocus::LEFT)
    {
        if (LEFT_PANEL_STARTX < x && x <= LEFT_PANEL_STARTX + LEFT_PANEL_WIDTH)
        {
            bool outOfRange = false;
            if (LEFT_PANEL_STARTY < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 1)
            {
                m_leftCursor = 0;
                m_leftSelect = 0 + m_leftBegin;
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 1 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 2)
            {
                m_leftCursor = 1;
                m_leftSelect = 1 + m_leftBegin;
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 2 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 3)
            {
                m_leftCursor = 2;
                m_leftSelect = 2 + m_leftBegin;
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 3 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 4)
            {
                m_leftCursor = 3;
                m_leftSelect = 3 + m_leftBegin;
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 4 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 5)
            {
                m_leftCursor = 4;
                m_leftSelect = 4 + m_leftBegin;
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 5 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 6)
            {
                m_leftCursor = 5;
                m_leftSelect = 5 + m_leftBegin;
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 6 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 7)
            {
                m_leftCursor = 6;
                m_leftSelect = 6 + m_leftBegin;
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 7 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 8)
            {
                m_leftCursor = 7;
                m_leftSelect = 7 + m_leftBegin;
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 8 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 9)
            {
                m_leftCursor = 8;
                m_leftSelect = 8 + m_leftBegin;
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 9 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 10)
            {
                m_leftCursor = 9;
                m_leftSelect = 9 + m_leftBegin;
            }
            else
            {
                outOfRange = true;
            }

            if (0 <= m_leftSelect && (size_t)m_leftSelect < m_leftList.size())
            {
                if (!outOfRange)
                {
                    m_SE->PlayClick();
                    result = Into();
                }
            }
            else
            {
                m_leftCursor = previousCursor;
                m_leftSelect = previousSelect;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        // do nothing
    }
    return result;
}

void PatchTestLib::Draw()
{
    // 背景
    m_sprBackground->DrawImage(0, 0);

    // 縦線
    m_sprVBar->DrawImage(350, 150);

    // 上部分の左に「インベントリ」、右側に「倉庫」と表示する
    m_font->DrawText_("食材リスト", 125, 80);

    m_font->DrawText_("食材名", 155, 160, 64);

    m_font->DrawText_("テスト状況", 855, 80);

    m_font->DrawText_("現在日時", 1155, 80, 64);

    m_font->DrawText_(m_CurrentDateTime, 1300, 80, 64);

    m_font->DrawText_("食材名", 455, 160, 64);
    m_font->DrawText_("テスト状況", 700, 160, 64);
    m_font->DrawText_("依頼日", 870, 160, 64);
    m_font->DrawText_("開始日", 1100, 160, 64);
    m_font->DrawText_("完了日", 1330, 160, 64);

    // 左の列のインベントリを表示
    if ((int)m_leftList.size() >= PANEL_ROW_MAX)
    {
        for (int i = m_leftBegin; i < m_leftBegin + PANEL_ROW_MAX; ++i)
        {
            m_font->DrawText_(m_leftList.at(i).GetName(),
                              LEFT_PANEL_STARTX,
                              LEFT_PANEL_STARTY + ((i - m_leftBegin) * PANEL_HEIGHT));
        }
    }
    else
    {
        for (std::size_t i = 0; i < m_leftList.size(); ++i)
        {
            m_font->DrawText_(m_leftList.at(i).GetName(),
                              LEFT_PANEL_STARTX,
                              LEFT_PANEL_STARTY + ((int)i * PANEL_HEIGHT));
        }
    }

    // 右の列にテスト状況を表示
    // 末尾の要素が一番上に表示される
    if ((int)m_rightList.size() >= PANEL_ROW_MAX)
    {
        for (int i = m_rightBegin; i < m_rightBegin + PANEL_ROW_MAX; ++i)
        {
            int work = (m_rightList.size() - 1) - i;

            int work2 = (i - m_rightBegin);

            m_font->DrawText_(m_rightList.at(work).GetName(),
                              RIGHT_PANEL_STARTX,
                              RIGHT_PANEL_STARTY + (work2 * PANEL_HEIGHT));

            m_font->DrawText_(m_rightList.at(work).GetResult(),
                              RIGHT_PANEL_STARTX + 300,
                              RIGHT_PANEL_STARTY + (work2 * PANEL_HEIGHT));

            m_font->DrawText_(m_rightList.at(work).GetDateReqStr(),
                              RIGHT_PANEL_STARTX + 470,
                              RIGHT_PANEL_STARTY + (work2 * PANEL_HEIGHT));

            m_font->DrawText_(m_rightList.at(work).GetDateStartStr(),
                              RIGHT_PANEL_STARTX + 700,
                              RIGHT_PANEL_STARTY + (work2 * PANEL_HEIGHT));

            m_font->DrawText_(m_rightList.at(work).GetDateEndStr(),
                              RIGHT_PANEL_STARTX + 930,
                              RIGHT_PANEL_STARTY + (work2 * PANEL_HEIGHT));
        }
    }
    else
    {
        for (size_t i = 0; i < m_rightList.size(); ++i)
        {
            int work = (m_rightList.size() - 1) - i;
            m_font->DrawText_(m_rightList.at(work).GetName(),
                              RIGHT_PANEL_STARTX,
                              RIGHT_PANEL_STARTY + ((int)i * PANEL_HEIGHT));

            m_font->DrawText_(m_rightList.at(work).GetResult(),
                              RIGHT_PANEL_STARTX + 300,
                              RIGHT_PANEL_STARTY + ((int)i * PANEL_HEIGHT));

            m_font->DrawText_(m_rightList.at(work).GetDateReqStr(),
                              RIGHT_PANEL_STARTX + 470,
                              RIGHT_PANEL_STARTY + ((int)i * PANEL_HEIGHT));

            m_font->DrawText_(m_rightList.at(work).GetDateStartStr(),
                              RIGHT_PANEL_STARTX + 700,
                              RIGHT_PANEL_STARTY + ((int)i * PANEL_HEIGHT));

            m_font->DrawText_(m_rightList.at(work).GetDateEndStr(),
                              RIGHT_PANEL_STARTX + 930,
                              RIGHT_PANEL_STARTY + ((int)i * PANEL_HEIGHT));

        }
    }

    // カーソルの表示
    if (m_eFocus == eFocus::LEFT)
    {
        if (m_leftCursor != -1)
        {
            m_sprCursor->DrawImage(LEFT_PANEL_STARTX - 30,
                                   LEFT_PANEL_STARTY + 5 + (m_leftCursor * 60));
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (m_rightCursor != -1)
        {
            m_sprCursor->DrawImage(RIGHT_PANEL_STARTX - 30,
                                   RIGHT_PANEL_STARTY + 5 + (m_rightCursor * 60));
        }
    }
}

void NSPatchTestLib::PatchTestLib::UpdateCursorPos()
{
    // カーソルの位置が範囲外なら範囲内に収まるように移動する。
    // リストが空の場合は、-1をセットする

    if (m_eFocus == eFocus::LEFT)
    {
        if (m_leftList.empty())
        {
            m_leftCursor = -1;
        }
        else
        {
            if ((size_t)m_leftSelect >= m_leftList.size())
            {
                m_leftSelect = m_leftList.size() - 1;
                --m_leftCursor;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (m_rightList.empty())
        {
            m_rightCursor = -1;
        }
        else
        {
            if ((size_t)m_rightSelect >= m_rightList.size())
            {
                m_rightSelect = m_rightList.size() - 1;
                --m_rightSelect;
            }
        }
    }
}

void NSPatchTestLib::TestItem::SetId(const int arg)
{
    m_id = arg;
}

int NSPatchTestLib::TestItem::GetId() const
{
    return m_id;
}

void NSPatchTestLib::TestItem::SetSubId(const int arg)
{
    m_idSub = arg;
}

int NSPatchTestLib::TestItem::GetSubId() const
{
    return m_idSub;
}

void NSPatchTestLib::TestItem::SetName(const std::string& arg)
{
    m_name = arg;
}

std::string NSPatchTestLib::TestItem::GetName() const
{
    return m_name;
}

void NSPatchTestLib::QueuedTestItem::SetName(const std::string& arg)
{
    m_name = arg;
}

std::string NSPatchTestLib::QueuedTestItem::GetName() const
{
    return m_name;
}

void NSPatchTestLib::QueuedTestItem::SetDateReq(const int year, const int month, const int day,
                                                const int hour, const int minute, const int second)
{
    m_reqYear = year;
    m_reqMonth = month;
    m_reqDay = day;
    m_reqHour = hour;
    m_reqMinute = minute;
    m_reqSecond = second;
}

void NSPatchTestLib::QueuedTestItem::GetDateReq(int* year, int* month, int* day,
                                                int* hour, int* minute, int* second)
{
    *year = m_reqYear;
    *month = m_reqMonth;
    *day = m_reqDay;
    *hour = m_reqHour;
    *minute = m_reqMinute;
    *second = m_reqSecond;
}

std::string NSPatchTestLib::QueuedTestItem::GetDateReqStr()
{
    return CreateDateTimeStr(m_reqYear,
                             m_reqMonth,
                             m_reqDay,
                             m_reqHour,
                             m_reqMinute,
                             m_reqSecond);
}

void NSPatchTestLib::QueuedTestItem::SetDateStart(const int year, const int month, const int day,
                                                  const int hour, const int minute, const int second)
{
    m_startYear = year;
    m_startMonth = month;
    m_startDay = day;
    m_startHour = hour;
    m_startMinute = minute;
    m_startSecond = second;
}

void NSPatchTestLib::QueuedTestItem::GetDateStart(int* year, int* month, int* day,
                                                  int* hour, int* minute, int* second)
{
    *year = m_startYear;
    *month = m_startMonth;
    *day = m_startDay;
    *hour = m_startHour;
    *minute = m_startMinute;
    *second = m_startSecond;
}

std::string NSPatchTestLib::QueuedTestItem::GetDateStartStr()
{
    return CreateDateTimeStr(m_startYear,
                             m_startMonth,
                             m_startDay,
                             m_startHour,
                             m_startMinute,
                             m_startSecond);
}

void NSPatchTestLib::QueuedTestItem::SetDateEnd(const int year, const int month, const int day,
                                                const int hour, const int minute, const int second)
{
    m_endYear = year;
    m_endMonth = month;
    m_endDay = day;
    m_endHour = hour;
    m_endMinute = minute;
    m_endSecond = second;
}

void NSPatchTestLib::QueuedTestItem::GetDateEnd(int* year, int* month, int* day,
                                                int* hour, int* minute, int* second)
{
    *year = m_endYear;
    *month = m_endMonth;
    *day = m_endDay;
    *hour = m_endHour;
    *minute = m_endMinute;
    *second = m_endSecond;
}

std::string NSPatchTestLib::QueuedTestItem::GetDateEndStr()
{
    return CreateDateTimeStr(m_endYear,
                             m_endMonth,
                             m_endDay,
                             m_endHour,
                             m_endMinute,
                             m_endSecond);
}

void NSPatchTestLib::QueuedTestItem::SetResult(const std::string& arg)
{
    m_result = arg;
}

std::string NSPatchTestLib::QueuedTestItem::GetResult() const
{
    return m_result;
}

std::string NSPatchTestLib::CreateDateTimeStr(const int y, const int M, const int d, const int h, const int m, const int s)
{
    if (y == 0)
    {
        return std::string();
    }

    std::string work;
    std::string work2;

    work += std::to_string(y) + "/";

    // 一桁数字だったら二桁文字にする
    if (M <= 9)
    {
        work += "0" + std::to_string(M) + "/";
    }
    else
    {
        work += std::to_string(M) + "/";
    }

    if (d <= 9)
    {
        work += "0" + std::to_string(d) + " ";
    }
    else
    {
        work += std::to_string(d) + " ";
    }

    if (h <= 9)
    {
        work += "0" + std::to_string(h) + ":";
    }
    else
    {
        work += std::to_string(h) + ":";
    }

    if (m <= 9)
    {
        work += "0" + std::to_string(m) + ":";
    }
    else
    {
        work += std::to_string(m) + ":";
    }

    if (s <= 9)
    {
        work += "0" + std::to_string(s);
    }
    else
    {

        work += std::to_string(s);
    }

    return work;
}
