#include "base/py_bindings.h"

#include "GW/trade/trade.h"
#include "GW/game_thread/game_thread.h"

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(PyTrade, m) {
    m.doc() = "Py4GW Trade bindings";

    m.def("open_trade_window", [](uint32_t agent_id) -> bool {
        GW::game_thread::Enqueue([agent_id]() {
            GW::trade::OpenTradeWindow(agent_id);
        });
        return true;
    }, py::arg("agent_id"));

    m.def("accept_trade", []() -> bool {
        GW::game_thread::Enqueue([]() {
            GW::trade::AcceptTrade();
        });
        return true;
    });

    m.def("cancel_trade", []() -> bool {
        GW::game_thread::Enqueue([]() {
            GW::trade::CancelTrade();
        });
        return true;
    });

    m.def("change_offer", []() -> bool {
        GW::game_thread::Enqueue([]() {
            GW::trade::ChangeOffer();
        });
        return true;
    });

    m.def("submit_offer", [](uint32_t gold) -> bool {
        GW::game_thread::Enqueue([gold]() {
            GW::trade::SubmitOffer(gold);
        });
        return true;
    }, py::arg("gold"));

    m.def("remove_item", [](uint32_t slot) -> bool {
        GW::game_thread::Enqueue([slot]() {
            GW::trade::RemoveItem(slot);
        });
        return true;
    }, py::arg("slot"));

    m.def("offer_item", [](uint32_t item_id, uint32_t quantity) -> bool {
        GW::game_thread::Enqueue([item_id, quantity]() {
            GW::trade::OfferItem(item_id, quantity);
        });
        return true;
    }, py::arg("item_id"), py::arg("quantity") = 0);

    m.def("is_item_offered", [](uint32_t item_id) -> bool {
        return GW::trade::IsItemOffered(item_id) != nullptr;
    }, py::arg("item_id"));
}
