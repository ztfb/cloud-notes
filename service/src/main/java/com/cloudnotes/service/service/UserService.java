package com.cloudnotes.service.service;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.User;
import org.springframework.web.bind.annotation.RequestBody;

public interface UserService {
    public Result login(String userName,String password);
    public Result register(User user);
    public Result update(@RequestBody User user);
    public Result logout(int id);
}
