#include "base/error_handling.h"

#include "GW/trade/trade.h"

#include "GW/context/context.h"
#include "GW/ctos/ctos.h"
#include "GW/ui/ui.h"

namespace GW::trade {

ui::Frame* GetTradeWindow() {
    return ui::GetFrameByLabel(L"DlgTrade");
}

uint32_t GetTradeState() {
    const auto c = Context::GetTradeContext();
    return c ? c->flags : 0;
}

bool OpenTradeWindow(uint32_t agent_id) {
    ui::packet::kSendWorldAction action{
        Constants::WorldActionId::InteractTrade,
        agent_id,
        false,
    };
    return ui::SendUIMessage(ui::UIMessage::kSendWorldAction, &action);
}

bool AcceptTrade() {
    const auto parent = GetTradeWindow();
    if ((GetTradeState() & Context::TradeContext::TRADE_INITIATED) == 0) {
        return ui::ButtonClick(ui::GetChildFrame(parent, 1));
    }
    return ui::ButtonClick(ui::GetChildFrame(parent, 2));
}

bool CancelTrade() {
    const auto parent = GetTradeWindow();
    if ((GetTradeState() & Context::TradeContext::TRADE_INITIATED) == 0) {
        return ui::ButtonClick(ui::GetChildFrame(parent, 2));
    }
    return ui::ButtonClick(ui::GetChildFrame(parent, 0xd)) || ui::ButtonClick(ui::GetChildFrame(parent, 1));
}

bool ChangeOffer() {
    return ui::ButtonClick(ui::GetChildFrame(GetTradeWindow(), 0));
}

bool SubmitOffer(uint32_t gold) {
    if (GetTradeState() != Context::TradeContext::TRADE_INITIATED)
        return false;

    // GoldEditPlayer handles 0x59 as "set current amount". The client
    // clamps the immediate value to the player's available gold and then
    // propagates its normal amount-changed event to DlgTrade.
    const auto frame = ui::GetFrameByLabel(L"GoldEditPlayer");
    return frame && ui::SendFrameUIMessage(
        frame,
        static_cast<ui::UIMessage>(0x59),
        reinterpret_cast<void*>(static_cast<uintptr_t>(gold)));
}

bool RemoveItem(uint32_t item_id) {
    if (GetTradeState() != Context::TradeContext::TRADE_INITIATED)
        return false;

    const auto offered_item = IsItemOffered(item_id);
    if (!offered_item)
        return false;

    // The current CartPlayer handler consumes a live cart-entry reference for
    // its remove action; GWCA's exported wrapper incorrectly supplies an item
    // id, so it cannot remove an offered item. The client ultimately emits the
    // exact three-dword packet below when a cart entry is removed manually.
    return CToS::QueuePacket({5u, offered_item->item_id, offered_item->quantity});
}

TradeItem* IsItemOffered(uint32_t item_id) {
    const auto ctx = Context::GetTradeContext();
    if (!ctx)
        return nullptr;
    auto& items = ctx->player.items;
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].item_id != item_id)
            continue;
        return &items[i];
    }
    return nullptr;
}

bool OfferItem(uint32_t item_id, uint32_t quantity) {
    if (GetTradeState() != Context::TradeContext::TRADE_INITIATED)
        return false;
    const auto frame = ui::GetFrameByLabel(L"CartPlayer");
    struct {
        uint32_t h0000 = 0;
        uint32_t h0004 = 2;
        uint32_t h0008 = 7;
        uint32_t* h000c;
        uint32_t h0010 = 0;
        uint32_t h0014;
        uint32_t h0018;
    } action;
    action.h000c = &action.h0014;
    action.h0014 = item_id;
    action.h0018 = quantity;

    return frame && !IsItemOffered(item_id) &&
           ui::SendFrameUIMessage(frame, static_cast<ui::UIMessage>(0x31), &action);
}

}  // namespace GW::trade
