#include <iostream>
#include <deque>
#include <tuple>
#include <functional>
#include <memory>
#include <algorithm>

//need more thinking...

class TaskSystem {
public:
	enum class Priority {
		Min,
		Low,
		Mid,
		High,
		Max,
	};
	struct Item;
	typedef std::shared_ptr<Item> SharedItem;

	struct Item {
		/*Do the something*/
		virtual bool Update() { return true; }
	};


	template<class FT>
	bool Update(FT F) {
		for (auto& In : Task) {
			F(this,In);
		}
		return true;
	}
	
	bool Update() {
		for (auto& In : Task) {
			std::get<1>(In)->Update();
		}
		return true;
	}

	bool Push(SharedItem In,Priority P) {
		Task.push_back({ P,In });
		std::stable_sort(Task.begin(), Task.end(), [](auto& A, auto& B) {return std::get<0>(A) > std::get<0>(B); });
		return true;
	}

	bool CallExit() {
		IsEND = true;
		return  true;
	}
	
	bool IsExit() {
		return IsEND;
	}
protected:
	typedef std::tuple<Priority, SharedItem> ITEM;
	std::deque<ITEM> Task;
	bool IsEND = false;
};

int main() {
	TaskSystem TS;
	TaskSystem::SharedItem TSSI = std::make_shared<TaskSystem::Item>();
	TaskSystem::SharedItem TSSI2 = std::make_shared<TaskSystem::Item>();
	TaskSystem::SharedItem TSSI3 = std::make_shared<TaskSystem::Item>();

	TS.Push(TSSI, TaskSystem::Priority::Mid);
	TS.Push(TSSI2, TaskSystem::Priority::Max);
	TS.Push(TSSI3, TaskSystem::Priority::Min);

	int N = 0;
	while (!TS.IsExit()) {
		TS.Update([&](TaskSystem* TS, const auto& o) {if (N == 100) { TS->CallExit(); } std::get<1>(o)->Update(); return true; });
		N++;
	}

	return 0;

}