package com.cloudnotes.service.controller;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.User;
import com.cloudnotes.service.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping(value = "/user")
@CrossOrigin
public class UserController {
    @Autowired
    private UserService userService;
    @GetMapping("/login")
    public Result login(String userName,String password){
        return userService.login(userName,password);
    }
    @PostMapping("/register")
    public Result register(@RequestBody User user){
        return userService.register(user);
    }
    @PostMapping("/update")
    public Result update(@RequestBody User user){return userService.update(user);}
    @GetMapping("/logout")
    public Result logout(int id){
        return userService.logout(id);
    }
}
