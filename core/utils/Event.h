#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>	

/// <summary>
/// Event template
/// </summary>
/// <typeparam name="T">
/// Type that will be used to distinguish the events, enum recommended
/// </typeparam>
template <typename T>
class Event
{
private:
	T type;
	std::string name;
	bool handled = false;
protected:
	virtual bool Handled() const { return handled; };
public:
	virtual ~Event() = default;

	/// <returns> Event name </returns>
	std::string GetName() const { return name; };
	/// <returns> Event type </returns>
	T GetType() const { return type; }
	/// <summary>
	/// Casts statically event to a different event type, 
	/// useful when you want to upbuild a custom event class 
	/// to give more data to listeners
	/// </summary>
	/// <typeparam name="EventType"> Other event type </typeparam>
	/// <returns> cast event </returns>
	template <typename EventType>
	EventType ToType() const {
		if (std::is_base_of<Event, EventType>::value)
			return static_cast<const EventType&> (*this);
		throw std::invalid_argument("wrong event type");
	}
	Event(T eventType, std::string eventName = "new event") : name(eventName), type(eventType) {};
};
/// <summary>
/// General listener template
/// </summary>
/// <typeparam name="T"> Event type </typeparam>
template <typename T>
struct EventListenerTemplate
{
	virtual ~EventListenerTemplate() = default;

	/// <summary>
	/// Virtual constructor
	/// </summary>
	/// <param name="isStatic"> Switch to tell if the function is static </param>
	EventListenerTemplate(bool isStatic) : isStatic(isStatic) {};
	/// <summary>
	/// Gets std::function object for attached function
	/// </summary>
	/// <returns></returns>
	virtual std::function<void(const Event<T>&)> GetFunction() const { return NULL; };
	/// <summary>
	/// Triggers the functions
	/// </summary>
	/// <param name="ev"></param>
	virtual void operator()(const Event<T>& ev) { GetFunction()(ev); };
	/// <summary>
	/// Casts listener to a different type
	/// </summary>
	/// <typeparam name="ListenerType"> Other listener type </typeparam>
	/// <returns></returns>
	template <typename ListenerType>
	ListenerType ToType() const {
		return static_cast<const ListenerType&> (*this);
	}
	/// <returns> true if the listener is static, false if non-static </returns>
	bool IsStatic() { return isStatic; };
private:
	bool isStatic = false;
};
/// <summary>
/// Wrapper around a static function
/// </summary>
/// <typeparam name="T">Event type</typeparam>
template <typename T>
struct EventListener : public EventListenerTemplate<T>
{
	/// <summary>
	/// Method to call
	/// </summary>
	void(* function)(const Event<T>&);
	/// <summary>
	/// Constructs listener from a listener from a method
	/// </summary>
	EventListener(void(*function)(const Event<T>&)) : EventListenerTemplate<T>(true), function(function) { };

	/// <summary>
	/// Constructs std::function object from the function
	/// </summary>
	/// <returns></returns>
	std::function<void(const Event<T>&)> GetFunction() const override {
		return std::function(function);
	}
	/// <summary>
	/// Compares if listeners are pointing to the same function
	/// </summary>
	/// <param name="other"></param>
	/// <returns>true if functions are the same</returns>
	bool operator== (const EventListener& other) const {
		return function == other.function;
	};
	/// <summary>
	/// Triggers the function
	/// </summary>
	/// <param name="ev"></param>
	void operator()(const Event<T>& ev) override {
		function(ev);
	};
};
/// <summary>
/// Wrapper around a non-static function
/// </summary>
/// <typeparam name="T"> Event type </typeparam>
/// <typeparam name="Sender"> Object type </typeparam>
template <typename T, class Sender>
struct EventListenerNonStatic : public EventListenerTemplate<T>
{
	/// <summary>
	/// Method to call
	/// </summary>
	void(Sender::*function)(const Event<T>&);
	/// <summary>
	/// Object on which the method will be called
	/// </summary>
	Sender& obj;
	/// <summary>
	/// Creates listener from a non-static function and object reference
	/// </summary>
	/// <param name="function"> Function to be called </param>
	/// <param name="obj"> Object to execute function on </param>
	EventListenerNonStatic(void(Sender::*function)(const Event<T>&), Sender& obj) : EventListenerTemplate<T>(false), function(function), obj(obj) { };
	/// <summary>
	/// Gets an std::function to the function
	/// </summary>
	/// <returns></returns>
	std::function<void(const Event<T>&)> GetFunction() const override {

		return std::function<void(const Event<T>&)>(std::bind(function, std::ref(obj), std::placeholders::_1));
	}
	/// <summary>
	/// Compares if listeners are pointing to the same function and on the same object
	/// </summary>
	/// <param name="other"> Other listener </param>
	/// <returns></returns>
	bool operator== (const EventListenerNonStatic& other) const {
		return (function == other.function && &obj == &(other.obj));
	};
	/// <summary>
	/// Triggers the function
	/// </summary>
	/// <param name="ev"> Other listener </param>
	void operator()(const Event<T>& ev) override {
		(obj.*function)(ev); 
	};
};

/// <summary>
/// Event dispatcher
/// </summary>
/// <typeparam name="T">Event type</typeparam>
template <typename T>
class EventDispatcher
{
	using func = EventListenerTemplate<T>;
	/// <summary>
	/// Contains all events and their listeners
	/// </summary>
	std::unordered_map<T, std::vector<std::unique_ptr<func>>> calls;

public:
	/// <summary>
	/// Adds a non-static listener
	/// </summary>
	/// <typeparam name="Sender"> Object type </typeparam>
	/// <param name="type"> Event type </param>
	/// <param name="function"> Function to add </param>
	/// <param name="obj"> Object that executes the function </param>
	/// <returns></returns>
	template <class Sender>
	//Sender obj is not const to allow listeners to change inside the object
	int AddListener(T type, void (Sender::* function)(const Event<T>&), Sender& obj)
	{
		std::unique_ptr<EventListenerNonStatic<T, Sender>> created = std::make_unique<EventListenerNonStatic<T, Sender>>(function, obj);
		calls[type].push_back(std::move(created));
		return calls[type].size() - 1;
	};
	/// <summary>
	/// Adds a static listener
	/// </summary>
	/// <param name="type"> Event type </param>
	/// <param name="function"> Function to add </param>
	/// <returns> Handle of the event (not required to remove the listener) </returns>
	int AddListener(T type, void (* function)(const Event<T>&))
	{
		std::unique_ptr<EventListener<T>> created = std::make_unique<EventListener<T>>(function);
		calls[type].push_back(std::move(created));
		return calls[type].size() - 1;
	};
	/// <summary>
	/// Finds location of a static listener for an event if present
	/// </summary>
	/// <param name="type"> Event type </param>
	/// <param name="function"> Function to search for </param>
	/// <returns> Iterator to the listener location </returns>
	std::vector<std::unique_ptr<func>>::iterator Find(T type, void (*function)(const Event<T>&))
	{
		EventListener<T> target(function);
		auto loc = calls[type].begin();
		for (; loc < calls[type].end(); loc++)
		{
			if ((*loc)->IsStatic())
			{
				EventListener<T> ptr = (*loc)->template ToType<EventListener<T>>();
				if (target == ptr)
					return loc;
			}
		}
		return calls[type].end();
	}
	/// <summary>
	/// Finds location of a static listener for an event if present
	/// </summary>
	/// <typeparam name="Sender"> Object type </typeparam>
	/// <param name="type"> Event type </param>
	/// <param name="function"> Function to search for </param>
	/// <param name="obj"> Object that executes the function </param>
	/// <returns> Iterator to the listener location </returns>
	template <class Sender>
	std::vector<std::unique_ptr<func>>::iterator Find(T type, void (Sender::*function)(const Event<T>&), Sender& obj)
	{
		EventListenerNonStatic<T,Sender> target(function, obj);
		auto loc = calls[type].begin();
		for (; loc < calls[type].end(); loc++)
		{
			if (!(*loc)->IsStatic())
			{
				EventListenerNonStatic<T,Sender> ptr = (*loc)->template ToType<EventListenerNonStatic<T,Sender>>();
				if (target == ptr)
					return loc;
			}
		}
		return calls[type].end();
	}
	/// <summary>
	/// Removes a non-static listener
	/// </summary>
	/// <param name="type"> Event type </param>
	/// <param name="function"> Function the remove </param>
	/// <returns></returns>
	bool RemoveListener(T type, void (*function)(const Event<T>&))
	{
		auto loc = Find(type, function);
		if (loc == calls[type].end())
		{
			std::cout << "couldn't remove static listener\n";
			return false;
		}
		calls[type].erase(loc);
		return true;
	}
	/// <summary>
	/// Removes a non-static listener
	/// </summary>
	/// <typeparam name="Sender"> Object type </typeparam>
	/// <param name="type"> Event type </param>
	/// <param name="function"> Function to remove </param>
	/// <param name="obj"> Object that executes the function </param>
	/// <returns></returns>
	template <class Sender>
		bool RemoveListener(T type, void (Sender::* function)(const Event<T>&), Sender& obj)
	{
		auto loc = Find<Sender>(type, function, obj);
		if (loc == calls[type].end())
		{
			std::cout << "couldn't remove nonstatic listener\n";
			return false;
		}
		calls[type].erase(loc);
		return true;
	}
	/// <summary>
	/// Trigger event
	/// </summary>
	/// <param name="event">
	/// Event to trigger
	/// </param>
	void Invoke(const Event<T>& event)
	{
		auto qualifier = event.GetType();
		auto iterator = calls.find(qualifier);
		if (iterator == calls.end())
		{
			//std::cout << "no listeners were found for an event -> event was ignored\n";
			return;
		}

		for (std::unique_ptr<func>& listener : iterator->second) {
			(*listener)(event);
		}
	}
};
